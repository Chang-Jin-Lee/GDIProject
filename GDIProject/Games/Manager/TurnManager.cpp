#include "TurnManager.h"

TurnManager::TurnManager()
{
	bStatic = true;
	CurrentTurn = ETurnState::PlayerTurn;
	Player = CreateDefaultSubobject<APlayerController>(TEXT("Player"));
}

TurnManager::~TurnManager()
{
	OwnerScene.reset();
	for (auto& m_tile : m_tiles)
	{
		for (auto& tile : m_tile)
		{
			tile.reset();
		}
	}
	Player.reset();
}

void TurnManager::Initialize()
{
	if (Player)
	{
		Player->OwnerScene = OwnerScene;
		Player->m_tiles = m_tiles;
		Player->Initialize();
	}
}

void TurnManager::Update()
{
	if (CurrentTurn == ETurnState::PlayerTurn)
	{
		Player->HandleInput();
	}
	// AI는 Update()는 굳이 매 프레임 하지 않음 (EndTurn()때 수행)
}

void TurnManager::Release()
{
	OwnerScene.reset();
	for (auto& m_tile : m_tiles)
	{
		for (auto& tile : m_tile)
		{
			tile.reset();
		}
	}
	Player.reset();
}
