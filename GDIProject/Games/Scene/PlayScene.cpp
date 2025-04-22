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

UPlayScene::UPlayScene()
{
	m_objects.clear();
	m_fPlayerCharacter.reset();
	m_fPlayerCharacter = nullptr;
	WorldBound = new FAABBBox(0.0f, 0.0f, Renderer::GetResolution().x, Renderer::GetResolution().y);
	quadTree = new FQuadTree(0, WorldBound);

	AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (g)
	{
		g->GameScore = 0;
	}

	std::wstring m_scoreuiname = L"m_scoreuiname";
	std::wstring m_remainTimeGuideuiname = L"m_remainTimeGuideui";
	std::wstring m_remainTimeuiname = L"m_remainTimeui";

	m_scoreui = NewObject<SUITextComponent>(m_scoreuiname);
	m_remainTimeGuideui = NewObject<SUITextComponent>(m_remainTimeGuideuiname);
	m_remainTimeui = NewObject<SUITextComponent>(m_remainTimeuiname);
}

UPlayScene::~UPlayScene()
{
	for (auto object : m_objects)
	{
		if (std::shared_ptr<UObject> _objet = std::dynamic_pointer_cast<UObject>(object.second)) _objet.reset();
	}
	m_objects.clear();
	m_fPlayerCharacter.reset();
	delete WorldBound;
	delete quadTree;
	m_scoreui.reset();
	m_remainTimeGuideui.reset();
	m_remainTimeui.reset();
}

void UPlayScene::Initialize()
{
	CharactersInitialize();
	TimeInitialize();
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
	m_fPlayerCharacter = NewObject<APlayerCharacter>(PlayerName);
	m_fPlayerCharacter->SetName(PlayerName.c_str());
	m_fPlayerCharacter->Initialize();

	for (int i = 0; i < m_enemyMaxSize; i++)
	{
		m_fEnemyCharacter = NewObject<AEnemyCharacter>(EnemyName);
		m_fEnemyCharacter->SetName(EnemyName.c_str());
		m_fEnemyCharacter->Initialize();
	}
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

	m_remainTimeui->m_content = (wchar_t*)malloc(sizeof(wchar_t) * 10);
	wchar_t gameremainTimeStr[10];
	swprintf_s(gameremainTimeStr, 10, L"%f", 10.0f);
	wcscpy_s(m_remainTimeui->m_content, 10, gameremainTimeStr);
}

void UPlayScene::UpdateCollisionDetection()
{
	quadTree->Clear();
	for (auto objectPair : m_objects)
	{
		if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(objectPair.second))
		{
			quadTree->Insert(actor, *actor->GetBoundBox());
		}
	}

	for (auto objectPair : m_objects)
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
						m_objects.erase(other->GetName());

						AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
						if (g)
						{
							g->GameScore++;
							wchar_t gameScoreStr[10];
							swprintf_s(gameScoreStr, 10, L"%d", g->GameScore);
							wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);
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
		UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
		m_fcountOneSecond = Time::GetTotalTime();
	}
}

void UPlayScene::UpdateUI()
{
	wchar_t gameScoreStr[10];
	swprintf_s(gameScoreStr, 10, L"%f", m_fFPSTime-m_fFPSLastTime);
	wcscpy_s(m_remainTimeui->m_content, 10, gameScoreStr);
}