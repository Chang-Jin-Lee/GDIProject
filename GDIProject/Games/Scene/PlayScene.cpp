#include "EndScene.h"
#include "PlayScene.h"
#include "WinScene.h"
#include <iostream>
#include <Runtime/Renderer/Renderer.h>
#include <Experiment/CollistionDetection.h>
#include <Input/Input.h>
#include <Classes/Scene/Scene.h>
#include <UI/UITextComponent.h>
#include <Math/Math.h>
#include "../Games.h"
#include "../Character/EnemyCharacter.h"
#include "../Player/TurnGameState.h"
#include "../Player/PlayerController.h"

UPlayScene::UPlayScene()
{
	for (auto objectMap : m_objects)
	{
		objectMap.clear();
	}
	m_PlayScene_Widget = CreateWidget<UPlayScene_Widget>(TEXT("PlaySceneWidget"), EUILAYER::HUD);
	m_PlayScene_Widget->m_spawnSettelerUnitButton->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Settler)); });
	m_PlayScene_Widget->m_spawnWarriorUnitButton->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Warrior)); });
	m_PlayScene_Widget->m_spawnArcherUnitButton->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Archer)); });
	m_PlayScene_Widget->m_endGameButton->SetVoidDelegate([this]() { GoNextScene(); });
	m_PlayScene_Widget->m_nextStageButton->SetVoidDelegate([this]() { NextTurn(); });

	m_tiles.assign(TILE_ROW_SIZE, std::vector<std::shared_ptr<ATile>>());
	for (int i = 0; i < TILE_ROW_SIZE; i++)
	{
		for (int j = 0; j < TILE_COL_SIZE; j++)
		{
			std::wstring str = TEXT("tile") + std::to_wstring(TILE_COL_SIZE * i + j);
			m_tiles[i].push_back(NewObject<ATile>(str, ESCENELAYER::GROUND));
			m_tiles[i][j].get()->SetName(str);
		}
	}
}

UPlayScene::~UPlayScene()
{
	for (auto objectMap : m_objects)
	{
		for (auto object : objectMap)
		{
			if (std::shared_ptr<UObject> _objet = std::dynamic_pointer_cast<UObject>(object.second))
			{
				_objet.reset();
			}
		}
	}
	m_objects.clear();
	m_PlayScene_Widget.reset();
	for (int i = 0; i < TILE_ROW_SIZE; i++)
	{
		for (int j = 0; j < TILE_COL_SIZE; j++)
		{
			m_tiles[i][j].reset();
		}
	}
	TurnMgr.reset();

	m_objects.clear();
}

void UPlayScene::Initialize()
{
	TileInitilize();
	TurnManagerInitilize();
	UIInitialize();
}

void UPlayScene::Update()
{
	__super::Update();
	//UpdateCollisionDetection();
	UpdateUI();
	UpdateInput();
}

void UPlayScene::LoadData()
{

}

void UPlayScene::Release()
{
	for (auto objectMap : m_objects)
	{
		for (auto object : objectMap)
		{
			if (std::shared_ptr<UObject> _objet = std::dynamic_pointer_cast<UObject>(object.second))
			{
				std::cout << "_objet : " << _objet.use_count() << '\n';
				_objet.reset();
			}
		}
	}
	m_objects.clear();
	m_PlayScene_Widget.reset();
	for (int i = 0; i < TILE_ROW_SIZE; i++)
	{
		for (int j = 0; j < TILE_COL_SIZE; j++)
		{
			std::cout << "m_tiles" << i << ' ' << j << " : " << m_tiles[i][j].use_count() << '\n';
			while (m_tiles[i][j].use_count() > 0)
			{
				m_tiles[i][j].reset();
			}
			m_tiles[i][j].reset();
		}
	}
	std::cout << "TurnMgr : " << TurnMgr.use_count() << '\n';
	TurnMgr.reset();
}

void UPlayScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UPlayScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
	}

	if (Input::IsKeyPressed(VK_LBUTTON))
	{
		if (Game::CheckWidgetPosition(Input::GetMousePosition()) == false)
		{
			FVector2 CameraPosition = Game::GetGameState()->GetMainCamera().get()->GetActorLocation();
			FVector2 index = ATile::GetIndexAtPosition(MousePosition + CameraPosition);
			MousePosition = Input::GetMousePosition() + CameraPosition;
		}
	}
}

void UPlayScene::UIInitialize()
{
	m_PlayScene_Widget->Initialize();
}

void UPlayScene::TileInitilize()
{
	for (int i = 0; i < TILE_ROW_SIZE; i++)
	{
		for (int j = 0; j < TILE_COL_SIZE; j++)
		{
			m_tiles[i][j].get()->m_etileType = static_cast<ETileType>((int)FRandom::GetRandomInRange(0, static_cast<int>(ETileType::Capital)));
			if (i % 2 == 0)
			{
				m_tiles[i][j].get()->SetActorLocation(j * m_tiles[i][j].get()->InitialTileSize.x, -i * m_tiles[i][j].get()->InitialTileSize.y / 3 + i * m_tiles[i][j].get()->InitialTileSize.y);
			}
			else
			{
				m_tiles[i][j].get()->SetActorLocation(m_tiles[i][j].get()->InitialTileSize.x * 0.5f + j * m_tiles[i][j].get()->InitialTileSize.x, -i * m_tiles[i][j].get()->InitialTileSize.y / 3 + i * m_tiles[i][j].get()->InitialTileSize.y);
			}
			m_tiles[i][j].get()->Initialize();
		}
	}

	int x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
	int y = (int)TILE_COL_SIZE * 0.8;
	m_tiles[x][y].get()->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
	m_tiles[x][y].get()->Initialize();

	x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
	m_tiles[x][y].get()->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
	m_tiles[x][y].get()->Initialize();

	x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
	m_tiles[x][y].get()->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
	m_tiles[x][y].get()->Initialize();

	x = (int)TILE_ROW_SIZE * 0.8;
	y = (int)FRandom::GetRandomInRange(0, TILE_COL_SIZE);
	m_tiles[x][y].get()->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
	m_tiles[x][y].get()->Initialize();

	x = (int)TILE_ROW_SIZE * 0.8;
	y = (int)FRandom::GetRandomInRange(0, TILE_COL_SIZE);
	m_tiles[x][y].get()->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
	m_tiles[x][y].get()->Initialize();
}

void UPlayScene::TurnManagerInitilize()
{
	TurnMgr = NewObject<TurnManager>(TEXT("TurnMgr"));
	TurnMgr->OwnerScene = shared_from_this();
	TurnMgr->m_tiles = m_tiles;
	TurnMgr->Initialize();
	if (g_TurnGameStateInstanceIsValid)
	{
		g_TurnGameStateInstance->m_bGameOver = false;
		g_TurnGameStateInstance->m_iTurnCount = 0;
		g_TurnGameStateInstance->m_iTurnMax = m_iMaxTurn;
		g_TurnGameStateInstance->m_bPlayerWin = false;
		g_TurnGameStateInstance->m_gGameScore = 0;
	}
}

void UPlayScene::UpdateCollisionDetection()
{
	//quadTree->Clear();
	//for (const auto& objectMap : m_objects)
	//{
	//	for (const auto& objectPair : objectMap)
	//	{
	//		if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(objectPair.second))
	//		{
	//			quadTree->Insert(actor, *actor->GetBoundBox());
	//		}
	//	}
	//}
	//
	//for (auto& objectMap : m_objects)
	//{
	//	for (auto& objectPair : objectMap)
	//	{
	//		std::shared_ptr<UObject> object = objectPair.second;
	//		// only collision detect Player to Enemy
	//		if (std::shared_ptr<APlayerCharacter> actor = std::dynamic_pointer_cast<APlayerCharacter>(object))
	//		{
	//			FAABBBox boundA = *actor->GetBoundBox();
	//			std::vector<std::shared_ptr<UObject>> TargetObject;
	//			quadTree->GetElements(boundA, TargetObject);
	//			if (boundA.IsValid() == false) continue;
	//			for (auto& tother : TargetObject)
	//			{
	//				if (std::shared_ptr<AEnemyCharacter> other = std::dynamic_pointer_cast<AEnemyCharacter>(tother))
	//				{
	//					FAABBBox boundB = *other->GetBoundBox();
	//					if (boundB.IsValid() == false) continue;
	//					if (Experiment::FCollisionDetector::AABBCollisionCheck(boundA, boundB))
	//					{
	//						// 원소 지우기
	//						std::wstring dd = other->GetName();
	//						objectMap.erase(other->GetName());
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void UPlayScene::UpdateUI()
{
	if (m_PlayScene_Widget->m_popupText->m_bVisible)
	{
		if (Time::GetTotalTime() - m_PlayScene_Widget->m_currentTime > m_PlayScene_Widget->m_PopUpTextDelay)
		{
			m_PlayScene_Widget->m_popupText->m_bVisible = false;
			m_PlayScene_Widget->m_popupRectangle->m_bVisible = false;
		}
	}
}

void UPlayScene::NextTurn()
{
	if (TurnMgr->GetCurrentTurn() == ETurnState::PlayerTurn)
	{
		CheckVictoryConditions();

		if (g_TurnGameStateInstanceIsValid)
		{
			g_TurnGameStateInstance->m_iTurnCount++;
			m_PlayScene_Widget->m_remainTurnui->m_content = std::to_wstring(g_TurnGameStateInstance->m_iTurnCount);
		}

		m_PlayScene_Widget->m_popupRectangle->m_brush->SetColor(Gdiplus::Color(10, 10, 222));
		PopUpUI(L"다음 턴으로 넘어갑니다.");
		ReadyForNextStage();
	}
}

void UPlayScene::SpawnUnit(int type)
{
	TurnMgr->GetPlayer()->SpawnAtPosition(MousePosition, static_cast<EUnitType>(type));
}

void UPlayScene::GoNextScene()
{
	Game::SetMouseDragState(false);
	UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
}

bool UPlayScene::CheckUnitActionCount()
{
	for (const auto& ch : TurnMgr->GetPlayer()->Units)
	{
		if (ch->ActionMaxCount > 0)
		{
			m_PlayScene_Widget->m_popupRectangle->m_brush->SetColor(Gdiplus::Color(222, 10, 10));
			PopUpUI(L"행동 수가 남아있습니다.");
			return false;
		}
	}
	return true;
}

void UPlayScene::CheckVictoryConditions()
{
	bool bVictory = true;
	for (const auto& ch : TurnMgr->GetPlayer()->Units)
	{
		FVector2 Index = ATile::GetIndexAtPosition(ch->GetActorLocation());
		int y = (int)Index.y;
		int x = (int)Index.x;
		if (m_tiles[x][y]->m_etileType != ETileType::Capital)
		{
			bVictory = false;
		}
		ch->ReadyForNextTurn();
	}
	if (bVictory)
	{
		UScene::ChangeScene<UWinScene>(Game::GetNextScenePtr());
	}
	else
	{
		if (g_TurnGameStateInstance->m_iTurnCount >= g_TurnGameStateInstance->m_iTurnMax)
		{
			UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
		}
	}
}

void UPlayScene::PopUpUI(const std::wstring& str)
{
	m_PlayScene_Widget->m_currentTime = Time::GetTotalTime();
	m_PlayScene_Widget->m_popupText->m_content = str;
	m_PlayScene_Widget->m_popupText->m_bVisible = true;
	m_PlayScene_Widget->m_popupRectangle->m_bVisible = true;
}

void UPlayScene::ReadyForNextStage()
{
	for (const auto& ch : TurnMgr->GetPlayer()->Units)
	{
		ch->ReadyForNextTurn();
	}

	// 5개의 타일이 랜덤하게 삭제.
	for (int i = 0; i < 5; i++)
	{
		int x = FRandom::GetRandomInRange(0, int(TILE_ROW_SIZE));
		int y = FRandom::GetRandomInRange(0, int(TILE_COL_SIZE));
		
		while (m_tiles[x][y]->bVisible == false || m_tiles[x][y]->unit != nullptr)
		{
			x = FRandom::GetRandomInRange(0, int(TILE_ROW_SIZE));
			y = FRandom::GetRandomInRange(0, int(TILE_COL_SIZE));
		}
		m_tiles[x][y]->bVisible = false;
	}
}