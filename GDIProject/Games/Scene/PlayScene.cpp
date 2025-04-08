#include "PlayScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>

void UPlayScene::Initialize()
{
	m_fPlayerCharacter = new APlayerCharacter();
	m_fPlayerCharacter->Initialize();
	m_fPlayerCharacter->SetActorLocation(50, 50);

	m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();
}

void UPlayScene::Update()
{
	Input();

	if (m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_frames)
	{
		Renderer::RenderImage(
			m_fPlayerCharacter
			->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]
			->m_frames[m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip]
			->m_frame,
			m_fPlayerCharacter->GetActorLocation().x,
			m_fPlayerCharacter->GetActorLocation().y,
			m_fPlayerCharacter
			->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]
			->m_frames[m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip]
			->m_frameScale.x,
			m_fPlayerCharacter
			->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]
			->m_frames[m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip]
			->m_frameScale.y
		);
	}

	m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
	if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
	{
		if (m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationMaxSize != 0)
		{
			m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip =
				(m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip + 1)
				% m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationMaxSize;
		}
		m_fcountOneSecondAnimationScene = Time::GetTotalTime();
	}
}

void UPlayScene::LoadData()
{

}

void UPlayScene::Release()
{

}

void UPlayScene::Input()
{

	if (Input::IsKeyPressed(VK_1))
	{
		m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Idle;
	}
	if (Input::IsKeyPressed(VK_2))
	{
		m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Attack;
	}
	if (Input::IsKeyPressed(VK_3))
	{
		m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Run;
	}

	if (Input::IsKeyDown(VK_RIGHT))
	{
		m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Right;
		//m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
		m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x + MoveSpeed * Time::GetElapsedTime(), m_fPlayerCharacter->GetActorLocation().y);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Left;
		//m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
		m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x - MoveSpeed * Time::GetElapsedTime(), m_fPlayerCharacter->GetActorLocation().y);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Bottom;
		//m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
		m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x, m_fPlayerCharacter->GetActorLocation().y + MoveSpeed * Time::GetElapsedTime());
	}
	if (Input::IsKeyDown(VK_UP))
	{
		m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Top;
		//m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
		m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x, m_fPlayerCharacter->GetActorLocation().y - MoveSpeed * Time::GetElapsedTime());
	}
}
