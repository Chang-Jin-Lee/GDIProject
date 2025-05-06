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
#include <Experiment/SmartCast.h>

UPlayScene::UPlayScene()
{
	for (auto objectMap : m_objects)
	{
		objectMap.clear();
	}
	m_PlayScene_Widget = CreateWidget<UPlayScene_Widget>(TEXT("PlaySceneWidget"), EUILAYER::HUD);
	if (const auto ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
	{
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_spawnSettelerUnitButton))
		{
			btn->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Settler)); });
		}
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_spawnWarriorUnitButton))
		{
			btn->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Warrior)); });
		}
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_spawnArcherUnitButton))
		{
			btn->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Archer)); });
		}
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_endGameButton))
		{
			btn->SetVoidDelegate([this]() { GoNextScene(); });
		}
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_nextStageButton))
		{
			btn->SetVoidDelegate([this]() { NextTurn(); });
		}
	}

	m_tiles.assign(TILE_ROW_SIZE, std::vector<std::weak_ptr<ATile>>());
	for (int i = 0; i < TILE_ROW_SIZE; i++)
	{
		for (int j = 0; j < TILE_COL_SIZE; j++)
		{
			std::wstring str = TEXT("tile") + std::to_wstring(TILE_COL_SIZE * i + j);
			m_tiles[i].push_back(NewObject<ATile>(str, ESCENELAYER::GROUND));
			if (const auto tile = Cast<ATile>(m_tiles[i][j]))
			{
				tile->SetName(str);
			}
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
		UScene::ChangeScene<UEndScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
	}

	if (Input::IsKeyPressed(VK_LBUTTON))
	{
		if (Game::CheckWidgetPosition(Input::GetMousePosition()) == false)
		{
			if (const auto cameraRef = Game::GetGameState()->GetMainCamera().lock())
			{
				FVector2 CameraPosition = cameraRef->GetActorLocation();
				FVector2 index = ATile::GetIndexAtPosition(MousePosition + CameraPosition);
				MousePosition = Input::GetMousePosition() + CameraPosition;
			}
		}
	}
}

void UPlayScene::UIInitialize()
{
	if (const auto widget = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
	{
		widget->Initialize();
	}
}

void UPlayScene::TileInitilize()
{
	for (int i = 0; i < TILE_ROW_SIZE; i++)
	{
		for (int j = 0; j < TILE_COL_SIZE; j++)
		{
			if (const auto tile = Cast<ATile>(m_tiles[i][j]))
			{
				tile->m_etileType = static_cast<ETileType>((int)FRandom::GetRandomInRange(0, static_cast<int>(ETileType::Capital)));
				if (i % 2 == 0)
				{
					tile->SetActorLocation(j * tile->InitialTileSize.x, -i * tile->InitialTileSize.y / 3 + i * tile->InitialTileSize.y);
				}
				else
				{
					tile->SetActorLocation(tile->InitialTileSize.x * 0.5f + j * tile->InitialTileSize.x, -i * tile->InitialTileSize.y / 3 + i * tile->InitialTileSize.y);
				}
				tile->Initialize();
			}
			
		}
	}

	int x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
	int y = (int)TILE_COL_SIZE * 0.8;
	if (const auto tile = Cast<ATile>(m_tiles[x][y]))
	{
		tile->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
		tile->Initialize();
	}

	x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
	if (const auto tile = Cast<ATile>(m_tiles[x][y]))
	{
		tile->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
		tile->Initialize();
	}

	x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
	if (const auto tile = Cast<ATile>(m_tiles[x][y]))
	{
		tile->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
		tile->Initialize();
	}

	x = (int)TILE_ROW_SIZE * 0.8;
	y = (int)FRandom::GetRandomInRange(0, TILE_COL_SIZE);
	if (const auto tile = Cast<ATile>(m_tiles[x][y]))
	{
		tile->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
		tile->Initialize();
	}

	x = (int)TILE_ROW_SIZE * 0.8;
	y = (int)FRandom::GetRandomInRange(0, TILE_COL_SIZE);
	if (const auto tile = Cast<ATile>(m_tiles[x][y]))
	{
		tile->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
		tile->Initialize();
	}
}

void UPlayScene::TurnManagerInitilize()
{
	TurnMgr = NewObject<TurnManager>(TEXT("TurnMgr"));
	if (const auto ref = TurnMgr.lock())
	{
		ref->OwnerScene = weak_from_this();
		ref->m_tiles = m_tiles;
		ref->Initialize();
	}

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
	if (const auto ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
	{
		if (const auto popupText = Cast<SUITextComponent>(ref->m_popupText))
		{
			if (popupText->m_bVisible)
			{
				if (Time::GetTotalTime() - ref->m_currentTime > ref->m_PopUpTextDelay)
				{
					popupText->m_bVisible = false;
					if (const auto popupRectangle = Cast<SUIButtonComponent>(ref->m_popupRectangle))
					{
						popupRectangle->m_bVisible = false;
					}
				}
			}
		}
		
	}
}

void UPlayScene::NextTurn()
{
	if (const auto ref = Cast<TurnManager>(TurnMgr))
	{
		if (ref->GetCurrentTurn() == ETurnState::PlayerTurn)
		{
			CheckVictoryConditions();

			if (const auto PlayScene_Widget_Ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
			{
				if (g_TurnGameStateInstanceIsValid)
				{
					g_TurnGameStateInstance->m_iTurnCount++;

					if (auto remainui = Cast<SUITextComponent>(PlayScene_Widget_Ref->m_remainTurnui))
					{
						remainui->m_content = std::to_wstring(g_TurnGameStateInstance->m_iTurnCount);
					}
				}
				if (auto popupRectangle = Cast<SUIButtonComponent>(PlayScene_Widget_Ref->m_popupRectangle))
				{
					popupRectangle->m_brush->SetColor(Gdiplus::Color(10, 10, 222));
				}
			}
			PopUpUI(L"다음 턴으로 넘어갑니다.");
			ReadyForNextStage();
		}
	}
	
}

void UPlayScene::SpawnUnit(int type)
{
	if (const auto ref = Cast<TurnManager>(TurnMgr))
	{
		if (auto player = Cast<APlayerController>(ref->GetPlayer()))
		{
			player->SpawnAtPosition(MousePosition, static_cast<EUnitType>(type));
		}
	}
}

void UPlayScene::GoNextScene()
{
	Game::SetMouseDragState(false);
	UScene::ChangeScene<UEndScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
}

bool UPlayScene::CheckUnitActionCount()
{
	if (const auto turnRef = Cast<TurnManager>(TurnMgr))
	{
		if (auto player = Cast<APlayerController>(turnRef->GetPlayer()))
		{
			for (const auto ch : player->Units)
			{
				if (auto chRef = Cast<APlayerCharacter>(ch))
				{
					if (chRef->ActionMaxCount > 0)
					{
						if (const auto PlayScene_Widget_Ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
						{
							if (auto popupRectangle = Cast<SUIButtonComponent>(PlayScene_Widget_Ref->m_popupRectangle))
							{
								popupRectangle->m_brush->SetColor(Gdiplus::Color(222, 10, 10));
							}
						}
						PopUpUI(L"행동 수가 남아있습니다.");
						return false;
					}
				}
				
			}
		}
	}
	return true;
}

void UPlayScene::CheckVictoryConditions()
{
	bool bVictory = true;

	if (auto TurnMgrRef = Cast<TurnManager>(TurnMgr))
	{
		if (auto playerRef = Cast<APlayerController>(TurnMgrRef->GetPlayer()))
		{
			for (const auto ch : playerRef->Units)
			{
				if (auto chRef = Cast<APlayerCharacter>(ch))
				{

					FVector2 Index = ATile::GetIndexAtPosition(chRef->GetActorLocation());
					int y = (int)Index.y;
					int x = (int)Index.x;
					if (auto tile = Cast<ATile>(m_tiles[x][y]))
					{
						if (tile->m_etileType != ETileType::Capital)
						{
							bVictory = false;
						}
					}
					chRef->ReadyForNextTurn();
				}
			}
		}
		
	}
	if (bVictory)
	{
		UScene::ChangeScene<UWinScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
	}
	else
	{
		if (g_TurnGameStateInstance->m_iTurnCount >= g_TurnGameStateInstance->m_iTurnMax)
		{
			UScene::ChangeScene<UEndScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
		}
	}
}

void UPlayScene::PopUpUI(const std::wstring& str)
{
	if (auto ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
	{
		ref->m_currentTime = Time::GetTotalTime();
		if (const auto text = Cast<SUITextComponent>(ref->m_popupText))
		{
			text->m_content = str;
			text->m_bVisible = true;
		}
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_popupRectangle))
		{
			btn->m_bVisible = true;
		}
	}
}

void UPlayScene::ReadyForNextStage()
{
	if (const auto& turnRef = Cast<TurnManager>(TurnMgr))
	{
		if (const auto& player = Cast<APlayerController>(turnRef->GetPlayer()))
		{
			for (const auto& ch : player->Units)
			{
				if (const auto& chRef = Cast<APlayerCharacter>(ch))
				{
					chRef->ReadyForNextTurn();
				}
			}
		}
	}

	// 5개의 타일이 랜덤하게 삭제.
	for (int i = 0; i < 5; i++)
	{
		if (g_TurnGameStateInstanceIsValid)
		{
			if (g_TurnGameStateInstance->m_iTurnCount > 25)
			{
				return;
			}
		}
		int x = FRandom::GetRandomInRange(0, int(TILE_ROW_SIZE));
		int y = FRandom::GetRandomInRange(0, int(TILE_COL_SIZE));
		
		if (auto tile = Cast<ATile>(m_tiles[x][y]))
		{
			while (tile->bVisible == false || tile->unit.expired() == false)
			{
				x = FRandom::GetRandomInRange(0, int(TILE_ROW_SIZE));
				y = FRandom::GetRandomInRange(0, int(TILE_COL_SIZE));
				tile = Cast<ATile>(m_tiles[x][y]);
			}
			tile->bVisible = false;
		}
	}
}