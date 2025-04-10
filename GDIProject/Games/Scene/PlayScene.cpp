#include "PlayScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "EndScene.h"
#include "../Games.h"

UPlayScene::UPlayScene()
{
}

UPlayScene::~UPlayScene()
{
	delete m_fPlayerCharacter;
}

void UPlayScene::Initialize()
{
	m_fPlayerCharacter = new APlayerCharacter();
	m_fPlayerCharacter->Initialize();
	m_fPlayerCharacter->SetActorLocation(50, 50);
	m_fPlayerCharacter->SetActorScale(2.5f, 2.5f);
	m_fcharacterRotationSpeed = 360;
	m_fPlayerCharacter->SetName((wchar_t*)L"플레이어 캐릭터");
	//m_fPlayerCharacter->SetActorSize(8.0f, 15.0f);
	//m_fPlayerCharacter->SetActorRotation(50);

	m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();
}

void UPlayScene::Update()
{
	if (m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_frames)
	{
		Renderer::RenderImage(
			m_fPlayerCharacter
			->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]
			->m_frames[m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip]
			->m_frame,
			m_fPlayerCharacter
		);
	}

	m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
	if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
	{
		if (m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationMaxSize != 0)
		{
			if (m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip == m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationMaxSize - 1)
			{
				if (m_fPlayerCharacter->animstate == APlayerCharacter::AnimationState::Attack)
				{
					m_fPlayerCharacter->m_bAttackAnimationPlaying = false;
				}
			}
			m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip =
				(m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip + 1)
				% m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationMaxSize;
		}
		m_fcountOneSecondAnimationScene = Time::GetTotalTime();
	}

	Input();

}

void UPlayScene::LoadData()
{

}

void UPlayScene::Release()
{

}

void UPlayScene::Input()
{
	if (Input::IsKeyPressed(VK_5))
	{
		UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
	}

	if (Input::IsKeyDown(VK_R))
	{
		float rotation = m_fPlayerCharacter->GetActorRotation();
		m_fPlayerCharacter->SetActorRotation(rotation + Time::GetElapsedTime() * m_fcharacterRotationSpeed);
	}

	if (Input::IsKeyDown(VK_SPACE))
	{
		if (m_fPlayerCharacter->m_bAttackAnimationPlaying == false)
		{
			m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
			m_fPlayerCharacter->m_bAttackAnimationPlaying = true;
			m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Attack;
		}
	}

	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_DOWN))
	{
		if(m_fPlayerCharacter->m_bAttackAnimationPlaying == false)
			m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Run;
		if (Input::IsKeyDown(VK_RIGHT))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Right;
			//m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x + MoveSpeed * Time::GetElapsedTime(), m_fPlayerCharacter->GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_LEFT))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Left;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x - MoveSpeed * Time::GetElapsedTime(), m_fPlayerCharacter->GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_DOWN))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Bottom;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x, m_fPlayerCharacter->GetActorLocation().y + MoveSpeed * Time::GetElapsedTime());
		}
		if (Input::IsKeyDown(VK_UP))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Top;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x, m_fPlayerCharacter->GetActorLocation().y - MoveSpeed * Time::GetElapsedTime());
		}
	}
	else
	{
		if(m_fPlayerCharacter->m_bAttackAnimationPlaying == false)
			m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Idle;
	}
}
