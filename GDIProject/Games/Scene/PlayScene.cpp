#include "PlayScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "EndScene.h"
#include "../Games.h"
#include <iostream>
#include "../Character/EnemyCharacter.h"
#include <Experiment/CollistionDetection.h>

UPlayScene::UPlayScene()
{
	m_objects.clear();
	m_fPlayerCharacter.reset();
	m_fPlayerCharacter = nullptr;
	WorldBound = new FAABBBox(0,0,Renderer::GetWidth(), Renderer::GetHeight());
	quadTree = new FQuadTree(0, WorldBound);
}

UPlayScene::~UPlayScene()
{
	m_objects.clear();
	m_fPlayerCharacter.reset();
	//delete m_fPlayerCharacter;
}

void UPlayScene::Initialize()
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

	m_fFPSLastTime = Time::GetTotalTime();
	m_fcountOneSecond = Time::GetTotalTime();
}

void UPlayScene::Update()
{
	quadTree->Clear();
	for (auto object : m_objects)
	{
		object->Update();

		if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object))
		{
			// QuadTree Update
			quadTree->Insert(actor, actor->GetBoundBox());
			//actor->SetColor(FColor(1.0f));
		}
	}

	for (auto object : m_objects)
	{
		if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object))
		{
			FAABBBox boundA = actor->GetBoundBox();
			std::vector<std::shared_ptr<UObject>> TargetObject;
			quadTree->GetElements(boundA, TargetObject);
			for (auto tother : TargetObject)
			{
				if (std::shared_ptr<AActor> other = std::dynamic_pointer_cast<AEnemyCharacter>(tother))
				{
					if (actor != other )
					{
						FAABBBox boundB = other->GetBoundBox();
						if (Experiment::FCollisionDetector::AABBCollisionCheck(boundA, boundB))
						{
							// 원소 지우기
							m_objects.erase(remove(m_objects.begin(), m_objects.end(), other), m_objects.end());
							//actor->SetColor(FColor(1, 0, 0, 1));
							//other->SetColor(FColor(1, 0, 0, 1));
						}
					}
				}
			}
		}
	}

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

	Input();
}

void UPlayScene::LoadData()
{
}

void UPlayScene::Release()
{
	m_objects.clear();
}

void UPlayScene::Input()
{
	if (Input::IsKeyPressed(VK_5))
	{
		UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
	}

	m_fPlayerCharacter->Input();
}
