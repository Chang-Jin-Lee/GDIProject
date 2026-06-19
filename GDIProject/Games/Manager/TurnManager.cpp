#include "TurnManager.h"

#include "../Core/AutoMove.h"
#include "../Core/GameCommand.h"
#include "../Core/NetworkProtocol.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include <Time/Time.h>

#include <cstdlib>
#include <limits>

namespace
{
bool IsMultiplayerMode(EMatchMode mode)
{
	return mode == EMatchMode::MultiplayerHost || mode == EMatchMode::MultiplayerClient;
}

bool IsLocalSlot(EMatchMode mode, EPlayerSlot slot)
{
	if (mode == EMatchMode::SinglePlayer)
	{
		return slot == EPlayerSlot::Player1;
	}
	if (mode == EMatchMode::MultiplayerHost)
	{
		return slot == EPlayerSlot::Player1;
	}
	return slot == EPlayerSlot::Player2;
}

int GridDistance(int rowA, int colA, int rowB, int colB)
{
	return std::abs(rowA - rowB) + std::abs(colA - colB);
}
}

TurnManager::TurnManager()
{
	bStatic = true;
	CurrentTurn = ETurnState::PlayerTurn;
	Player = CreateDefaultSubobject<APlayerController>(TEXT("Player1"));
	Player2 = CreateDefaultSubobject<APlayerController>(TEXT("Player2"));
	Config = GameConfig::LoadFromResource();
	RemainingTurnSeconds = Config.GameRules.PvpTurnSeconds;
}

TurnManager::~TurnManager()
{

}

void TurnManager::Initialize()
{
	Config = GameConfig::LoadFromResource();
	RemainingTurnSeconds = Config.GameRules.PvpTurnSeconds;
	CurrentPhase = ETurnPhase::Player1Turn;
	CurrentTurn = ETurnState::PlayerTurn;
	bHandshakeSent = false;

	const MatchSettings& settings = GetMatchSettings();
	if (settings.Mode == EMatchMode::MultiplayerHost)
	{
		Network = std::make_unique<NetworkSession>();
		Network->Host(static_cast<unsigned short>(settings.Port));
		CurrentPhase = ETurnPhase::WaitingForConnection;
	}
	else if (settings.Mode == EMatchMode::MultiplayerClient)
	{
		Network = std::make_unique<NetworkSession>();
		Network->Connect(settings.RemoteIp, static_cast<unsigned short>(settings.Port));
		CurrentPhase = ETurnPhase::WaitingForConnection;
	}

	if (const auto PlayerRef = Player.lock())
	{
		PlayerRef->OwnerScene = OwnerScene;
		PlayerRef->m_tiles = m_tiles;
		PlayerRef->SetCommandSink([this](GameCommand command) { SendNetworkCommand(command); });
		PlayerRef->InitializeForSlot(EPlayerSlot::Player1);
	}
	if (const auto Player2Ref = Player2.lock())
	{
		Player2Ref->OwnerScene = OwnerScene;
		Player2Ref->m_tiles = m_tiles;
		Player2Ref->SetCommandSink([this](GameCommand command) { SendNetworkCommand(command); });
		Player2Ref->InitializeForSlot(EPlayerSlot::Player2);
	}
}

void TurnManager::Update()
{
	const EMatchMode mode = GetMatchSettings().Mode;
	PollNetwork();

	if (IsMultiplayerMode(mode) && CurrentPhase != ETurnPhase::GameOver && CurrentPhase != ETurnPhase::WaitingForConnection)
	{
		TurnTimerAccumulator += Time::GetElapsedTime();
		while (TurnTimerAccumulator >= 1.0)
		{
			TurnTimerAccumulator -= 1.0;
			--RemainingTurnSeconds;
		}
		if (RemainingTurnSeconds <= 0)
		{
			if (IsLocalSlot(mode, GetActiveSlot()))
			{
				ApplyTimeoutPenalty();
				EndActiveTurn();
			}
			return;
		}
	}

	const EPlayerSlot activeSlot = GetActiveSlot();
	if (activeSlot == EPlayerSlot::None || !IsLocalSlot(mode, activeSlot))
	{
		return;
	}

	if (const auto activePlayer = GetActivePlayer().lock())
	{
		activePlayer->UpdateMovement(Time::GetElapsedTime());
		if (!activePlayer->bIsUnitMoving)
		{
			activePlayer->HandleInput();
		}
	}
}

void TurnManager::Release()
{
	__super::Release();
	std::vector<std::vector<std::weak_ptr<ATile>>>().swap(m_tiles);
}

std::weak_ptr<APlayerController> TurnManager::GetActivePlayer()
{
	return GetActiveSlot() == EPlayerSlot::Player2 ? Player2 : Player;
}

EPlayerSlot TurnManager::GetActiveSlot() const
{
	if (CurrentPhase == ETurnPhase::Player1Turn)
	{
		return EPlayerSlot::Player1;
	}
	if (CurrentPhase == ETurnPhase::Player2Turn)
	{
		return EPlayerSlot::Player2;
	}
	return EPlayerSlot::None;
}

void TurnManager::EndActiveTurn(bool sendNetworkCommand)
{
	const EMatchMode mode = GetMatchSettings().Mode;
	if (sendNetworkCommand && IsMultiplayerMode(mode) && Network && Network->IsConnected())
	{
		SendNetworkCommand(GameCommand::EndTurn(g_TurnGameStateInstanceIsValid ? g_TurnGameStateInstance->m_iTurnCount : 0, GetActiveSlot()));
	}

	RemainingTurnSeconds = Config.GameRules.PvpTurnSeconds;
	TurnTimerAccumulator = 0.0;

	if (CurrentPhase == ETurnPhase::Player1Turn)
	{
		CurrentPhase = ETurnPhase::Player2Turn;
		CurrentTurn = ETurnState::AITurn;
		if (auto player2Ref = Player2.lock())
		{
			player2Ref->ReadyUnitsForNextTurn();
		}
		if (mode == EMatchMode::SinglePlayer)
		{
			ResolveAITurn();
			CurrentPhase = ETurnPhase::Player1Turn;
			CurrentTurn = ETurnState::PlayerTurn;
			if (auto playerRef = Player.lock())
			{
				playerRef->ReadyUnitsForNextTurn();
			}
		}
		return;
	}

	CurrentPhase = ETurnPhase::Player1Turn;
	CurrentTurn = ETurnState::PlayerTurn;
	if (auto playerRef = Player.lock())
	{
		playerRef->ReadyUnitsForNextTurn();
	}
}

void TurnManager::SkipSelectedUnit(bool sendNetworkCommand)
{
	if (auto activePlayer = GetActivePlayer().lock())
	{
		if (auto unit = activePlayer->SelectedUnit.lock())
		{
			unit->ActionRemainCount = 0;
			unit->bSkipTurn = true;
			if (sendNetworkCommand)
			{
				SendNetworkCommand(GameCommand::Skip(g_TurnGameStateInstanceIsValid ? g_TurnGameStateInstance->m_iTurnCount : 0, GetActiveSlot(), unit->StableId));
			}
		}
	}
}

void TurnManager::ApplyTimeoutPenalty(bool sendNetworkCommand)
{
	auto activePlayer = GetActivePlayer().lock();
	auto opponent = (GetActiveSlot() == EPlayerSlot::Player1 ? Player2 : Player).lock();
	if (!activePlayer || !opponent)
	{
		return;
	}

	std::vector<std::shared_ptr<APlayerCharacter>> enemies = opponent->GetLivingUnits();
	std::vector<std::vector<TerrainTile>> terrain;
	for (int row = 0; row < static_cast<int>(m_tiles.size()); ++row)
	{
		std::vector<TerrainTile> terrainRow;
		for (int col = 0; col < static_cast<int>(m_tiles[row].size()); ++col)
		{
			TerrainTile tileData;
			tileData.Row = row;
			tileData.Col = col;
			if (auto tile = m_tiles[row][col].lock())
			{
				tileData.Type = tile->m_etileType;
				tileData.Visible = tile->bVisible;
				tileData.Occupied = tile->unit.expired() == false;
				tileData.Capital = tile->m_etileType == ETileType::Capital;
			}
			terrainRow.push_back(tileData);
		}
		terrain.push_back(terrainRow);
	}

	for (auto& unit : activePlayer->GetLivingUnits())
	{
		if (!unit || unit->ActionRemainCount <= 0)
		{
			continue;
		}

		int enemyRow = unit->row;
		int enemyCol = unit->col;
		int bestDistance = (std::numeric_limits<int>::max)();
		for (const auto& enemy : enemies)
		{
			const int distance = GridDistance(unit->row, unit->col, enemy->row, enemy->col);
			if (distance < bestDistance)
			{
				bestDistance = distance;
				enemyRow = enemy->row;
				enemyCol = enemy->col;
			}
		}

		AutoMoveRequest request;
		request.UnitRow = unit->row;
		request.UnitCol = unit->col;
		request.RemainingAction = unit->ActionRemainCount;
		request.NearestEnemyRow = enemyRow;
		request.NearestEnemyCol = enemyCol;
		request.TargetRow = enemyRow;
		request.TargetCol = enemyCol;
		request.Tiles = terrain;

		const AutoMoveChoice choice = ChooseUnfavorableAutoMove(request, Config);
		if (choice.HasMove)
		{
			if (activePlayer->TryMoveUnitToIndex(unit, choice.Row, choice.Col, false) && sendNetworkCommand)
			{
				SendNetworkCommand(GameCommand::Move(g_TurnGameStateInstanceIsValid ? g_TurnGameStateInstance->m_iTurnCount : 0, GetActiveSlot(), unit->StableId, choice.Row, choice.Col));
			}
		}
		unit->ActionRemainCount = 0;
		unit->bSkipTurn = true;
	}
}

void TurnManager::ResolveAITurn()
{
	auto aiPlayer = Player2.lock();
	auto humanPlayer = Player.lock();
	if (!aiPlayer || !humanPlayer)
	{
		return;
	}

	const std::vector<std::shared_ptr<APlayerCharacter>> enemies = humanPlayer->GetLivingUnits();
	for (auto& unit : aiPlayer->GetLivingUnits())
	{
		if (!unit || unit->ActionRemainCount <= 0 || enemies.empty())
		{
			continue;
		}

		std::shared_ptr<APlayerCharacter> nearest;
		int nearestDistance = (std::numeric_limits<int>::max)();
		for (const auto& enemy : enemies)
		{
			const int distance = GridDistance(unit->row, unit->col, enemy->row, enemy->col);
			if (distance < nearestDistance)
			{
				nearest = enemy;
				nearestDistance = distance;
			}
		}

		if (!nearest)
		{
			continue;
		}

		if (aiPlayer->TryAttackUnit(unit, nearest))
		{
			continue;
		}

		int bestRow = unit->row;
		int bestCol = unit->col;
		int bestDistance = nearestDistance;
		const int* dx = (unit->row % 2 == 0) ? even_dx : odd_dx;
		const int* dy = (unit->row % 2 == 0) ? even_dy : odd_dy;
		for (int dir = 0; dir < 6; ++dir)
		{
			const int nextRow = unit->row + dy[dir];
			const int nextCol = unit->col + dx[dir];
			if (!ATile::IsValidIndex(nextRow, nextCol))
			{
				continue;
			}
			const int distance = GridDistance(nextRow, nextCol, nearest->row, nearest->col);
			if (distance < bestDistance)
			{
				bestDistance = distance;
				bestRow = nextRow;
				bestCol = nextCol;
			}
		}

		if (bestRow != unit->row || bestCol != unit->col)
		{
			aiPlayer->TryMoveUnitToIndex(unit, bestRow, bestCol, false);
			aiPlayer->TryAttackUnit(unit, nearest);
		}
		unit->ActionRemainCount = 0;
	}
}

void TurnManager::PollNetwork()
{
	if (!Network)
	{
		return;
	}

	Network->Poll();
	const EMatchMode mode = GetMatchSettings().Mode;
	if (CurrentPhase == ETurnPhase::WaitingForConnection && Network->IsConnected())
	{
		if (mode == EMatchMode::MultiplayerClient && !bHandshakeSent)
		{
			Network->SendLine("HELLO|version=1|role=client");
			bHandshakeSent = true;
		}
		else if (mode == EMatchMode::MultiplayerHost && !bHandshakeSent)
		{
			Network->SendLine("WELCOME|slot=Player2|seed=123456|mapHash=8F6A21C0");
			Network->SendLine("START|mode=Multiplayer|active=Player1");
			CurrentPhase = ETurnPhase::Player1Turn;
			CurrentTurn = ETurnState::PlayerTurn;
			bHandshakeSent = true;
		}
	}

	std::string line;
	while (Network->PopLine(line))
	{
		const NetworkLine parsed = ParseNetworkLine(line);
		if (!parsed.Valid)
		{
			continue;
		}

		if (parsed.Name == "START")
		{
			CurrentPhase = ETurnPhase::Player1Turn;
			CurrentTurn = ETurnState::PlayerTurn;
			continue;
		}

		if (parsed.Name == "COMMAND")
		{
			const CommandParseResult command = ParseCommand(line);
			if (command.Success)
			{
				ApplyRemoteCommand(command.Command);
			}
		}
	}
}

void TurnManager::SendNetworkCommand(GameCommand command)
{
	const EMatchMode mode = GetMatchSettings().Mode;
	if (!IsMultiplayerMode(mode) || !Network || !Network->IsConnected())
	{
		return;
	}

	if (command.Turn <= 0 && g_TurnGameStateInstanceIsValid)
	{
		command.Turn = g_TurnGameStateInstance->m_iTurnCount;
	}
	if (command.Slot == EPlayerSlot::None)
	{
		command.Slot = GetActiveSlot();
	}

	Network->SendLine(SerializeCommand(command));
}

void TurnManager::ApplyRemoteCommand(const GameCommand& command)
{
	std::shared_ptr<APlayerController> commandPlayer = (command.Slot == EPlayerSlot::Player2 ? Player2 : Player).lock();
	std::shared_ptr<APlayerController> opponent = (command.Slot == EPlayerSlot::Player2 ? Player : Player2).lock();
	if (!commandPlayer)
	{
		return;
	}

	if (command.Type == EGameCommandType::Move)
	{
		if (auto unit = commandPlayer->FindUnitByStableId(command.UnitId))
		{
			commandPlayer->TryMoveUnitToIndex(unit, command.Row, command.Col, false);
		}
		return;
	}

	if (command.Type == EGameCommandType::Attack)
	{
		if (opponent)
		{
			auto attacker = commandPlayer->FindUnitByStableId(command.UnitId);
			auto target = opponent->FindUnitByStableId(command.TargetUnitId);
			commandPlayer->TryAttackUnit(attacker, target);
		}
		return;
	}

	if (command.Type == EGameCommandType::Skip)
	{
		if (auto unit = commandPlayer->FindUnitByStableId(command.UnitId))
		{
			unit->ActionRemainCount = 0;
			unit->bSkipTurn = true;
		}
		return;
	}

	if (command.Type == EGameCommandType::EndTurn && GetActiveSlot() == command.Slot)
	{
		EndActiveTurn(false);
	}
}
