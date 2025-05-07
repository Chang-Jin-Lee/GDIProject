#include "TurnManager.h"

TurnManager::TurnManager()
{
	bStatic = true;
	CurrentTurn = ETurnState::PlayerTurn;
	Player = CreateDefaultSubobject<APlayerController>(TEXT("Player"));
}

TurnManager::~TurnManager()
{

}

void TurnManager::Initialize()
{
	if (const auto PlayerRef = Player.lock())
	{
		PlayerRef->OwnerScene = OwnerScene;
		PlayerRef->m_tiles = m_tiles;
		PlayerRef->Initialize();
	}
}

void TurnManager::Update()
{
	if (CurrentTurn == ETurnState::PlayerTurn)
	{
		if (const auto PlayerRef = Player.lock())
		{
			PlayerRef->HandleInput();
		}
	}
	// AI는 Update()는 굳이 매 프레임 하지 않음 (EndTurn()때 수행)
}

void TurnManager::Release()
{
	__super::Release();
	std::vector<std::vector<std::weak_ptr<ATile>>>().swap(m_tiles);
}
