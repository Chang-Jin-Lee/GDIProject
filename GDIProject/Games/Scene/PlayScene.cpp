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
#include <UI/UIButtonComponent.h>

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
	__super::Release();
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

    // 마우스 휠로 줌 인/아웃 (위 = 확대, 아래 = 축소)
    {
        int wheel = Input::GetWheelDelta(); // 프레임당 휠 변화량(예: +120/-120)
        if (wheel != 0)
        {
            if (const auto cameraRef = Game::GetGameState()->GetMainCamera().lock())
            {
                float step = (wheel > 0) ? 1.1f : 0.9f;
                FVector2 currentScale = cameraRef->GetCameraScale();
                float zoom = currentScale.x * step;
                if (zoom < 0.3f) zoom = 0.3f;
                if (zoom > 4.0f) zoom = 4.0f;
                cameraRef->SetActorScale(zoom, zoom);
            }
        }
    }

    if (Input::IsKeyPressed(VK_LBUTTON))
	{
		if (Game::CheckWidgetPosition(Input::GetMousePosition()) == false)
		{
			if (const auto cameraRef = Game::GetGameState()->GetMainCamera().lock())
			{
				FVector2 camPos = cameraRef->GetCameraLocation();
				float zoom = cameraRef->GetCameraScale().x;
				if (zoom <= 0.0f) zoom = 1.0f;
				FVector2 worldMouse = camPos + Input::GetMousePosition() / zoom;
				FVector2 index = ATile::GetIndexAtPosition(worldMouse);
				MousePosition = worldMouse;
			}
		}
	}
}

void UPlayScene::UIInitialize()
{
	if (const auto widget = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
	{
		widget->Initialize();
        // Skip Turn 버튼
        if (auto btn = Cast<SUIButtonComponent>(widget->m_skipTurnButton))
        {
            btn->SetVoidDelegate([this]() {
                if (const auto turnRef = Cast<TurnManager>(TurnMgr))
                {
                    if (auto player = Cast<APlayerController>(turnRef->GetPlayer()))
                    {
                        if (const auto unit = player->SelectedUnit.lock())
                        {
                            unit->bSkipTurn = true;
                        }
                    }
                }
            });
        }
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

	std::vector<std::pair<int, int>> indexVector;
	for (int i = 0; i < 3; i++)
	{
		int x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
		int y = (int)TILE_COL_SIZE * 0.8;
		while (std::find(indexVector.begin(), indexVector.end(), std::make_pair(x, y)) != indexVector.end())
		{
			x = (int)FRandom::GetRandomInRange(0, TILE_ROW_SIZE);
			y = (int)TILE_COL_SIZE * 0.8;
		}
		indexVector.emplace_back(x, y);

		if (const auto tile = Cast<ATile>(m_tiles[x][y]))
		{
			tile->m_etileType = static_cast<ETileType>(static_cast<int>(ETileType::Capital));
			tile->Initialize();
		}
	}
	std::vector<std::pair<int, int>>().swap(indexVector);
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
            if (popupText->IsVisible())
            {
                // shake during popup lifetime
                double elapsed = Time::GetTotalTime() - ref->m_currentTime;
                const double duration = ref->m_PopUpTextDelay;
                if (elapsed <= duration)
                {
                    float phase = float(elapsed * ref->m_ShakeFrequency);
                    float dx = std::sin(phase) * ref->m_ShakeAmplitude;
                    float dy = std::cos(phase) * ref->m_ShakeAmplitude;
                    if (auto rect = Cast<SUIButtonComponent>(ref->m_popupRectangle))
                    {
                        if (!ref->m_popupBaseCaptured)
                        {
                            auto bp = rect->GetPosition();
                            ref->m_popupRectBaseX = bp.x; ref->m_popupRectBaseY = bp.y;
                            auto tp = popupText->GetPosition();
                            ref->m_popupTextBaseX = tp.x; ref->m_popupTextBaseY = tp.y;
                            ref->m_popupBaseCaptured = true;
                        }
                        rect->SetPosition({ ref->m_popupRectBaseX + dx, ref->m_popupRectBaseY + dy });
                    }
                    popupText->SetPosition({ ref->m_popupTextBaseX + dx, ref->m_popupTextBaseY + dy });
                }
                else
                {
                    // hide and reset positions
                    popupText->SetVisible(false);
                    if (auto rect = Cast<SUIButtonComponent>(ref->m_popupRectangle))
                    {
                        rect->SetVisible(false);
                        if (ref->m_popupBaseCaptured)
                            rect->SetPosition({ ref->m_popupRectBaseX, ref->m_popupRectBaseY });
                    }
                    if (ref->m_popupBaseCaptured)
                        popupText->SetPosition({ ref->m_popupTextBaseX, ref->m_popupTextBaseY });
                    ref->m_popupBaseCaptured = false;
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
            // block if any unit still has remaining actions
            if (CheckUnitActionCount() == false)
            {
                return;
            }
            CheckVictoryConditions();

			if (const auto PlayScene_Widget_Ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
			{
				if (g_TurnGameStateInstanceIsValid)
				{
					g_TurnGameStateInstance->m_iTurnCount++;

                    if (auto remainui = Cast<SUITextComponent>(PlayScene_Widget_Ref->m_remainTurnui))
					{
                        remainui->SetContent(std::to_wstring(g_TurnGameStateInstance->m_iTurnCount));
					}
				}
				if (auto popupRectangle = Cast<SUIButtonComponent>(PlayScene_Widget_Ref->m_popupRectangle))
				{
                    if (auto brush = popupRectangle->GetBrush()) brush->SetColor(Gdiplus::Color(10, 10, 222));
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
                    if (chRef->bSkipTurn) continue;
                    if (chRef->ActionRemainCount > 0)
					{
						if (const auto PlayScene_Widget_Ref = Cast<UPlayScene_Widget>(m_PlayScene_Widget))
						{
                            if (auto popupRectangle = Cast<SUIButtonComponent>(PlayScene_Widget_Ref->m_popupRectangle))
							{
                                if (auto brush = popupRectangle->GetBrush()) brush->SetColor(Gdiplus::Color(222, 10, 10));
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
					//chRef->ReadyForNextTurn();
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
            text->SetContent(str);
            text->SetVisible(true);
		}
		if (const auto btn = Cast<SUIButtonComponent>(ref->m_popupRectangle))
		{
            btn->SetVisible(true);
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
					chRef->bSkipTurn = false;
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
			while (tile->bVisible == false || tile->unit.expired() == false || tile->m_etileType == ETileType::Capital)
			{
				x = FRandom::GetRandomInRange(0, int(TILE_ROW_SIZE));
				y = FRandom::GetRandomInRange(0, int(TILE_COL_SIZE));
				tile = Cast<ATile>(m_tiles[x][y]);
			}
			tile->bVisible = false;
		}
	}
}