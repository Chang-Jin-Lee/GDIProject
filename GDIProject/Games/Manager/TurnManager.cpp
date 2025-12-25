#include "TurnManager.h"
#include <Time/Time.h>

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
			// �̵� ���̸� �Է��� ���� �ʰ� �̵� ������Ʈ�� ����
			PlayerRef->UpdateMovement(Time::GetElapsedTime());
			if (!PlayerRef->bIsUnitMoving)
			{
				PlayerRef->HandleInput();
			}
		}
	}
	// AI�� Update()�� ���� �� ������ ���� ���� (EndTurn()�� ����)
}

void TurnManager::Release()
{
	__super::Release();
	std::vector<std::vector<std::weak_ptr<ATile>>>().swap(m_tiles);
}
