#include "EndScene.h"
#include "PlayScene.h"
#include <iostream>
#include <Runtime/Renderer/Renderer.h>
#include <Experiment/CollistionDetection.h>
#include <Input/Input.h>
#include "../Games.h"
#include "../Character/EnemyCharacter.h"
#include "../Player/dGameState.h"
#include <UI/UITextComponent.h>
#include <Math/Math.h>
#include <Classes/Scene/Scene.h>

UPlayScene::UPlayScene()
{
	for (auto objectMap : m_objects)
	{
		objectMap.clear();
	}

	m_fPlayerCharacter.reset();
	m_fPlayerCharacter = nullptr;
	WorldBound = new FAABBBox(0.0f, 0.0f, Renderer::GetResolution().x, Renderer::GetResolution().y);
	quadTree = new FQuadTree(0, WorldBound);

	AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (g)
	{
		g->m_gGameScore = 0;
	}

	m_fPlayerCharacter = NewObject<APlayerCharacter>(PlayerName, ESCENELAYER::CHARACTER);
	m_ScoreWidget = CreateWidget<UScoreWidget>(TEXT("ScoreWidget"));
	m_PlayScene_Widget = CreateWidget<UPlayScene_Widget>(TEXT("PlaySceneWidget"), EUILAYER::BUTTON);
	m_PlayScene_Widget->m_Button->SetVoidDelegate([this]() { printTest(); });
	//m_PlayScene_Widget->m_Button = UPlayScene::printTest;

	m_tiles.assign(TILE_COL_SIZE, std::vector<std::shared_ptr<ATile>>());
	for (int i = 0; i < TILE_COL_SIZE; i++)
	{
		for (int j = 0; j < TILE_ROW_SIZE; j++)
		{
			m_tiles[i].push_back(NewObject<ATile>(TEXT("tile"), ESCENELAYER::GROUND));
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
	m_fPlayerCharacter.reset();
	delete WorldBound;
	delete quadTree;
	m_ScoreWidget.reset();
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
	UIInitialize();

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
				//m_tiles[i][j].get()->SetActorLocation(j * size.x, -i * size.y / 3 + i * size.y);
				m_tiles[i][j].get()->SetActorLocation(j * m_tiles[i][j].get()->InitialTileSize.x, -i * m_tiles[i][j].get()->InitialTileSize.y / 3 +  i * m_tiles[i][j].get()->InitialTileSize.y);
			}
			else
			{
				//m_tiles[i][j].get()->SetActorLocation(size.x * 0.5f + j * size.x, -i * size.y / 3 + i * size.y);
				m_tiles[i][j].get()->SetActorLocation(m_tiles[i][j].get()->InitialTileSize.x * 0.5f + j * m_tiles[i][j].get()->InitialTileSize.x, -i * m_tiles[i][j].get()->InitialTileSize.y/3 + i * m_tiles[i][j].get()->InitialTileSize.y);
			}
		}
	}
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
}

void UPlayScene::TimeInitialize()
{
	m_fFPSLastTime = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();
}

void UPlayScene::CharactersInitialize()
{
	m_fPlayerCharacter->SetName(PlayerName.c_str());
	m_fPlayerCharacter->Initialize();

	//for (int i = 0; i < m_enemyMaxSize; i++)
	//{
	//	m_fEnemyCharacter = NewObject<AEnemyCharacter>(EnemyName, ESCENELAYER::CHARACTER);
	//	m_fEnemyCharacter->SetName(EnemyName.c_str());
	//	m_fEnemyCharacter->Initialize();
	//}
}

void UPlayScene::UIInitialize()
{
	m_ScoreWidget->Initialize();
	m_PlayScene_Widget->Initialize();
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
				for (auto tother : TargetObject)
				{
					if (std::shared_ptr<AEnemyCharacter> other = std::dynamic_pointer_cast<AEnemyCharacter>(tother))
					{
						FAABBBox boundB = *other->GetBoundBox();
						if (Experiment::FCollisionDetector::AABBCollisionCheck(boundA, boundB))
						{
							// 원소 지우기
							objectMap.erase(other->GetName());

							AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
							if (g)
							{
								g->m_gGameScore++;
								m_ScoreWidget->m_scoreui->m_content = std::to_wstring(g->m_gGameScore);
							}
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
	if (m_fFPSTime >= m_fFPSLastTime)
	{
		m_PlayScene_Widget->m_remainTimeui->m_content = std::to_wstring(m_fFPSTime - m_fFPSLastTime);
	}
}

void UPlayScene::printTest()
{
	printf("test\n");

	m_fPlayerCharacter = NewObject<APlayerCharacter>(PlayerName, ESCENELAYER::CHARACTER);
	m_fPlayerCharacter->SetName(PlayerName.c_str());
	m_fPlayerCharacter->Initialize();
	AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (g)
	{
		m_fPlayerCharacter->SetActorLocation(g->GetMainCamera().get()->GetActorLocation() + Renderer::GetResolution()/2);
	}
}
