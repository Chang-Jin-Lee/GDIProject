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

	m_scoreui = NewObject<SUITextComponent>(TEXT("scoreuiname"));
	m_remainTimeGuideui = NewObject<SUITextComponent>(TEXT("remainTimeGuideui"));
	m_remainTimeui = NewObject<SUITextComponent>(TEXT("remainTimeui"));

	m_tiles.assign(TILE_COL_SIZE, std::vector<std::shared_ptr<ATile>>());
	for (int i = 0; i < TILE_COL_SIZE; i++)
	{
		for (int j = 0; j < TILE_ROW_SIZE; j++)
		{
			m_tiles[i].push_back(NewObject<ATile>(TEXT("tile_0"), ELAYER::GROUND));
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
	m_scoreui.reset();
	m_remainTimeGuideui.reset();
	m_remainTimeui.reset();
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
				m_tiles[i][j].get()->SetActorLocation(j * size.x, -i * size.y / 3 +  i * size.y);
			}
			else
			{
				m_tiles[i][j].get()->SetActorLocation(size.x * 0.5f + j * size.x, -i * size.y/3 + i * size.y);
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
	m_fPlayerCharacter = NewObject<APlayerCharacter>(PlayerName, ELAYER::CHARACTER);
	m_fPlayerCharacter->SetName(PlayerName.c_str());
	m_fPlayerCharacter->Initialize();

	//for (int i = 0; i < m_enemyMaxSize; i++)
	//{
	//	m_fEnemyCharacter = NewObject<AEnemyCharacter>(EnemyName, ELAYER::CHARACTER);
	//	m_fEnemyCharacter->SetName(EnemyName.c_str());
	//	m_fEnemyCharacter->Initialize();
	//}
}

void UPlayScene::UIInitialize()
{
	float uiwidth = 60;
	float uiheith = 30;
	m_scoreui->Initialize
	(
		nullptr,
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.1)),
		FVector2(uiwidth, uiheith)
	);
	m_scoreui->m_content = (wchar_t*)malloc(sizeof(wchar_t) * 10);
	m_scoreui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	wchar_t gameScoreStr[10];
	swprintf_s(gameScoreStr, 10, L"%d", 0);
	wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);

	uiwidth = 150;
	uiheith = 40;
	m_remainTimeGuideui->Initialize
	(
		(wchar_t*)L"남은 시간 : ",
		18,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.35), int(Renderer::GetResolution().y * 0.15)),
		FVector2(uiwidth, uiheith)
	);
	m_remainTimeGuideui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	uiwidth = 60;
	uiheith = 30;
	m_remainTimeui->Initialize
	(
		nullptr,
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.15)),
		FVector2(uiwidth, uiheith)
	);
	m_remainTimeui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	m_remainTimeui->m_content = (wchar_t*)malloc(sizeof(wchar_t) * 10);

	wchar_t gameremainTimeStr[10];
	swprintf_s(gameremainTimeStr, 10, L"%f", 10.0f);
	wcscpy_s(m_remainTimeui->m_content, 10, gameremainTimeStr);
}

void UPlayScene::UpdateCollisionDetection()
{
	quadTree->Clear();
	for (auto objectMap : m_objects)
	{
		for (auto objectPair : objectMap)
		{
			if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(objectPair.second))
			{
				quadTree->Insert(actor, *actor->GetBoundBox());
			}
		}
	}

	for (auto objectMap : m_objects)
	{
		for (auto objectPair : objectMap)
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
								wchar_t gameScoreStr[10];
								swprintf_s(gameScoreStr, 10, L"%d", g->m_gGameScore);
								wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);
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
	wchar_t gameScoreStr[10];
	swprintf_s(gameScoreStr, 10, L"%f", m_fFPSTime-m_fFPSLastTime);
	wcscpy_s(m_remainTimeui->m_content, 10, gameScoreStr);

	//// 카메라 부착
	//m_scoreui.get()->m_Position = m_scoreui.get()->m_RelativePosition + Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
	//m_remainTimeui.get()->m_Position = m_remainTimeui.get()->m_RelativePosition + Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
	//m_remainTimeGuideui.get()->m_Position = m_remainTimeGuideui.get()->m_RelativePosition + Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
}