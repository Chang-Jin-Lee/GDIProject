#include "EndScene.h"
#include "PlayScene.h"
#include <iostream>
#include <Runtime/Renderer/Renderer.h>
#include <Experiment/CollistionDetection.h>
#include <Input/Input.h>
#include "../Games.h"
#include "../Character/EnemyCharacter.h"
#include "../Player/dGameState.h"

UPlayScene::UPlayScene()
{
	m_objects.clear();
	m_fPlayerCharacter.reset();
	m_fPlayerCharacter = nullptr;
	WorldBound = new FAABBBox(0,0, Renderer::GetResolution().x, Renderer::GetResolution().y);
	quadTree = new FQuadTree(0, WorldBound);

	AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (g)
	{
		g->GameScore = 0;
	}
	m_scoreui = new SUIText();
}

UPlayScene::~UPlayScene()
{
	m_objects.clear();
	m_fPlayerCharacter.reset();
	delete m_scoreui;
}

void UPlayScene::Initialize()
{
	CharactersInitialize();
	TimeInitialize();
	UIInitialize();
}

void UPlayScene::Update()
{
	UpdateCollisionDetection();
	Renderer::RenderTextUI(m_scoreui, Renderer::GetResolution().x * 0.5, Renderer::GetResolution().y * 0.1);
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
	if (Input::IsKeyPressed(VK_SPACE))
	{
		UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
	}

	m_fPlayerCharacter->Input();
}

void UPlayScene::TimeInitialize()
{
	m_fFPSLastTime = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();
}

void UPlayScene::CharactersInitialize()
{
	m_fPlayerCharacter = std::make_shared<APlayerCharacter>();
	m_fPlayerCharacter->Initialize();
	m_objects.push_back(m_fPlayerCharacter);

	for (int i = 0; i < m_enemyMaxSize; i++)
	{
		m_fEnemyCharacter = std::make_shared<AEnemyCharacter>();
		m_fEnemyCharacter->SetName((wchar_t*)L"적 캐릭터");
		m_fEnemyCharacter->Initialize();
		m_objects.push_back(m_fEnemyCharacter);
	}
}

void UPlayScene::UIInitialize()
{
	int uiwidth = 60;
	int uiheith = 30;
	m_scoreui->Initialize
	(
		nullptr,
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(-uiwidth / 2, -uiheith / 2),
		FVector2(uiwidth, uiheith)
	);
	m_scoreui->m_content = (wchar_t*)malloc(sizeof(wchar_t) * 10);
	wchar_t gameScoreStr[10];
	swprintf_s(gameScoreStr, 10, L"%d", 0);
	wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);
}

void UPlayScene::UpdateCollisionDetection()
{
	quadTree->Clear();
	for (auto object : m_objects)
	{
		object->Update();

		if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object))
		{
			quadTree->Insert(actor, actor->GetBoundBox());
		}
	}

	for (auto object : m_objects)
	{
		// only collision detect Player to Enemy
		if (std::shared_ptr<APlayerCharacter> actor = std::dynamic_pointer_cast<APlayerCharacter>(object))
		{
			FAABBBox boundA = actor->GetBoundBox();
			std::vector<std::shared_ptr<UObject>> TargetObject;
			quadTree->GetElements(boundA, TargetObject);
			for (auto tother : TargetObject)
			{
				if (std::shared_ptr<AEnemyCharacter> other = std::dynamic_pointer_cast<AEnemyCharacter>(tother))
				{
					FAABBBox boundB = other->GetBoundBox();
					if (Experiment::FCollisionDetector::AABBCollisionCheck(boundA, boundB))
					{
						// 원소 지우기
						m_objects.erase(remove(m_objects.begin(), m_objects.end(), other), m_objects.end());
						printf("%f,  %f", other->GetActorLocation().x, other->GetActorLocation().y);
						AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
						if (g)
						{
							g->GameScore++;
							wchar_t gameScoreStr[10];
							swprintf_s(gameScoreStr, 10, L"%d", g->GameScore);
							wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);
							std::cout << g->GameScore << '\n';
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
	if (m_fFPSLastTime >= m_fFPSTime)	// 1 초에 한 번씩
	{
		//UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
		//m_fEnemyCharacter = std::make_shared<AEnemyCharacter>();
		//m_fEnemyCharacter->Initialize();
		//SetName((wchar_t*)L"적 캐릭터_Default" + m_countEnemy++);
		//m_objects.push_back(m_fEnemyCharacter);
		//m_fcountOneSecond = Time::GetTotalTime();
	}
}
