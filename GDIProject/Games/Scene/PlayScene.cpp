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

	//m_fPlayerCharacter.reset();
	//m_fPlayerCharacter = nullptr;
	WorldBound = new FAABBBox(0.0f, 0.0f, Renderer::GetResolution().x, Renderer::GetResolution().y);
	quadTree = new FQuadTree(0, WorldBound);

	if (g_TurnGameStateInstanceIsValid)
	{
		g_TurnGameStateInstance->m_gGameScore = 0;
	}

	//m_fPlayerCharacter = NewObject<APlayerCharacter>(m_fPlayerCharacter->GetUnitTypeString(), ESCENELAYER::CHARACTER);
	m_PlayScene_Widget = CreateWidget<UPlayScene_Widget>(TEXT("PlaySceneWidget"), EUILAYER::BUTTON);
	//m_PlayScene_Widget->m_spawnSettelerUnitButton->SetVoidDelegate([this]() { SpawnUnit(); });
	m_PlayScene_Widget->m_spawnSettelerUnitButton->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Settler)); });
	m_PlayScene_Widget->m_spawnWarriorUnitButton->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Warrior)); });
	m_PlayScene_Widget->m_spawnArcherUnitButton->SetVoidDelegate([this]() { SpawnUnit(static_cast<int>(EUnitType::Archer)); });
	m_PlayScene_Widget->m_endGameButton->SetVoidDelegate([this]() { GoNextScene(); });
	m_PlayScene_Widget->m_nextStageButton->SetVoidDelegate([this]() { NextTurn(); });
	//m_PlayScene_Widget->m_Button->SetVoidDelegate(UPlayScene::printTest);
	//m_PlayScene_Widget->m_Button = UPlayScene::printTest;

	m_tiles.assign(TILE_COL_SIZE, std::vector<std::shared_ptr<ATile>>());
	for (int i = 0; i < TILE_COL_SIZE; i++)
	{
		for (int j = 0; j < TILE_ROW_SIZE; j++)
		{
			std::wstring str = TEXT("tile") + std::to_wstring(TILE_ROW_SIZE * i + j);
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
			if (std::shared_ptr<UObject> _objet = std::dynamic_pointer_cast<UObject>(object.second)) _objet.reset();
		}
	}
	m_objects.clear();
	if (m_fPlayerCharacter.get() != nullptr)
		m_fPlayerCharacter.reset();
	delete WorldBound;
	delete quadTree;
	m_PlayScene_Widget.reset();
	for (int i = 0; i < TILE_COL_SIZE; i++)
	{
		for (int j = 0; j < TILE_ROW_SIZE; j++)
		{
			m_tiles[i][j].reset();
		}
	}
}

void UPlayScene::Initialize()
{
	CharactersInitialize();
	TimeInitialize();
	TileInitilize();
	TurnManagerInitilize();
	UIInitialize();
}

void UPlayScene::Update()
{
	__super::Update();
	UpdateCollisionDetection();
	UpdateTime();
	UpdateUI();
	UpdateInput();
}

void UPlayScene::LoadData()
{

}

void UPlayScene::Release()
{
	m_objects.clear();
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

			std::cout << "Input::GetMousePosition() : " << MousePosition.x << ' ' << MousePosition.y << '\n';
			std::cout << "Input::GetMousePosition() index : " << index.x << "  " << index.y << '\n';
		}
	}
}

void UPlayScene::TimeInitialize()
{
	m_fFPSLastTime = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();
}

void UPlayScene::CharactersInitialize()
{
	//m_fPlayerCharacter->SetName(PlayerName.c_str());
	//m_fPlayerCharacter->Initialize();

	//for (int i = 0; i < m_enemyMaxSize; i++)
	//{
	//	std::wstring str = EnemyName + std::to_wstring(i);
	//	m_fEnemyCharacter = NewObject<AEnemyCharacter>(str, ESCENELAYER::CHARACTER);
	//	m_fEnemyCharacter->SetName(str);
	//	m_fEnemyCharacter->Initialize();
	//}
}

void UPlayScene::UIInitialize()
{
	m_PlayScene_Widget->Initialize();
}

void UPlayScene::TileInitilize()
{
	for (int i = 0; i < TILE_COL_SIZE; i++)
	{
		for (int j = 0; j < TILE_ROW_SIZE; j++)
		{
			m_tiles[i][j].get()->m_etileType = static_cast<ETileType>((int)FRandom::GetRandomInRange(0, static_cast<int>(ETileType::MAX)));
			//m_tiles[i][j].get()->m_etileType = ETileType::Plain;
			m_tiles[i][j].get()->Initialize();
			FVector2 size = m_tiles[i][j].get()->GetActorSize();
			if (i % 2 == 0)
			{
				m_tiles[i][j].get()->SetActorLocation(j * m_tiles[i][j].get()->InitialTileSize.x, -i * m_tiles[i][j].get()->InitialTileSize.y / 3 + i * m_tiles[i][j].get()->InitialTileSize.y);
			}
			else
			{
				m_tiles[i][j].get()->SetActorLocation(m_tiles[i][j].get()->InitialTileSize.x * 0.5f + j * m_tiles[i][j].get()->InitialTileSize.x, -i * m_tiles[i][j].get()->InitialTileSize.y / 3 + i * m_tiles[i][j].get()->InitialTileSize.y);
			}
		}
	}
}

void UPlayScene::TurnManagerInitilize()
{
	//TurnMgr = std::make_shared<TurnManager>();
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
	}
}

void UPlayScene::UpdateCollisionDetection()
{
	quadTree->Clear();
	for (const auto& objectMap : m_objects)
	{
		for (const auto& objectPair : objectMap)
		{
			if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(objectPair.second))
			{
				quadTree->Insert(actor, *actor->GetBoundBox());
			}
		}
	}

	for (auto& objectMap : m_objects)
	{
		for (auto& objectPair : objectMap)
		{
			std::shared_ptr<UObject> object = objectPair.second;
			// only collision detect Player to Enemy
			if (std::shared_ptr<APlayerCharacter> actor = std::dynamic_pointer_cast<APlayerCharacter>(object))
			{
				FAABBBox boundA = *actor->GetBoundBox();
				std::vector<std::shared_ptr<UObject>> TargetObject;
				quadTree->GetElements(boundA, TargetObject);
				if (boundA.IsValid() == false) continue;
				for (auto& tother : TargetObject)
				{
					if (std::shared_ptr<AEnemyCharacter> other = std::dynamic_pointer_cast<AEnemyCharacter>(tother))
					{
						FAABBBox boundB = *other->GetBoundBox();
						if (boundB.IsValid() == false) continue;
						if (Experiment::FCollisionDetector::AABBCollisionCheck(boundA, boundB))
						{
							// 원소 지우기
							std::wstring dd = other->GetName();
							objectMap.erase(other->GetName());
							//m_objects[other.get()->]
							//tother.reset();
							//m_objects[other->RenderLayer].erase(other->GetName());
						}
					}
				}
			}
		}
	}
}

void UPlayScene::UpdateTime()
{
	// 10초 뒤에 씬 전환
	m_fFPSLastTime = Time::GetTotalTime() - m_fcountOneSecond;
	if (m_fFPSLastTime >= m_fFPSTime)	// 10 초 지나면 넘어감.
	{
		//UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
		m_fcountOneSecond = Time::GetTotalTime();
	}
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
		printf("nextTurn! ");
		if (CheckUnitActionCount() == false)
		{
			return;
		}
		CheckVictoryConditions();
		if (g_TurnGameStateInstanceIsValid)
		{
			m_PlayScene_Widget->m_remainTurnui->m_content = std::to_wstring(g_TurnGameStateInstance->m_iTurnCount);
		}
		m_PlayScene_Widget->m_popupRectangle->m_brush->SetColor(Gdiplus::Color(10, 10, 222));
		PopUpUI(L"다음 턴으로 넘어갑니다.");
		ReadyForNextStage();
	}
}

void UPlayScene::SpawnUnit(int type)
{
	std::cout << MousePosition.x << ' ' << MousePosition.y << " Spawend! " << '\n';
	TurnMgr->GetPlayer()->SpawnAtPosition(MousePosition, static_cast<EUnitType>(type));
	//m_fPlayerCharacter = NewObject<APlayerCharacter>(APlayerCharacter::GetUnitTypeString(type) + std::to_wstring(Time::GetElapsedTime()), ESCENELAYER::CHARACTER);
	//m_fPlayerCharacter->SetName(APlayerCharacter::GetUnitTypeString(type).c_str());
	//m_fPlayerCharacter->SetUnitType(type);
	//m_fPlayerCharacter->Initialize();
	//if (g_TurnGameStateInstanceIsValid)
	//{
	//	m_fPlayerCharacter->SetActorLocation(g_TurnGameStateInstance->GetMainCamera().get()->GetActorLocation() + Renderer::GetResolution() / 2);
	//}
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
		if (ch->ActionCount > 0)
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
	if (g_TurnGameStateInstanceIsValid)
	{
		g_TurnGameStateInstance->m_iTurnCount++;

		// 1. 턴 수 제한
		if (g_TurnGameStateInstance->m_iTurnCount >= g_TurnGameStateInstance->m_iTurnMax)
		{
			printf("턴 제한 도달! 승패 자동 판정!\n");
			g_TurnGameStateInstance->m_bGameOver = true;
			// 점수 높은 쪽 승리 처리 가능
			if (g_TurnGameStateInstance->m_bPlayerWin)
			{
				Game::SetMouseDragState(false);
				UScene::ChangeScene<UWinScene>(Game::GetNextScenePtr());
			}
			else
			{
				Game::SetMouseDragState(false);
				UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
			}
			return;
		}

		// 2. AI 모두 도시 없음 → 플레이어 승리
		bool bAllAIKilled = true;
		for (auto& ai : TurnMgr->GetAIPlayers())
		{
			if (!ai->Cities.empty())
			{
				bAllAIKilled = false;
				break;
			}
		}

		if (bAllAIKilled)
		{
			printf("🎉 플레이어 승리!\n");
			g_TurnGameStateInstance->m_bGameOver = true;
			g_TurnGameStateInstance->m_bPlayerWin = true;
			return;
		}

		// 3. 플레이어 도시 모두 파괴 → 플레이어 패배
		if (TurnMgr->GetPlayer()->Cities.empty())
		{
			printf("💀 플레이어 패배!\n");
			g_TurnGameStateInstance->m_bGameOver = true;
			g_TurnGameStateInstance->m_bPlayerWin = false;
			return;
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

}