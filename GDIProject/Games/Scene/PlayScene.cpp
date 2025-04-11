#include "PlayScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "EndScene.h"
#include "../Games.h"
#include <iostream>

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

	int uiwidth = 120;
	int uiheith = 20;
	m_fPlayerCharacter->m_textui->Initialize
	(
		m_fPlayerCharacter->GetName(),
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255,255,255),
		FVector2(-50, -20),
		FVector2(uiwidth, uiheith),
		Gdiplus::FontStyleBold, 
		Gdiplus::UnitPoint,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringTrimmingNone
	);

	std::cout << uiwidth << ' ' << uiheith << '\n';
	//m_fPlayerCharacter->SetActorSize(8.0f, 15.0f);
	//m_fPlayerCharacter->SetActorRotation(50);

	m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();
}

void UPlayScene::Update()
{
	FVector2 mouseclick = Game::GetLMouseClickPosition();

	std::cout << mouseclick.x << ' ' << mouseclick.y << '\n';
	std::cout << m_fPlayerCharacter->GetActorLocation().x << ' ' << m_fPlayerCharacter->GetActorLocation().y << '\n';

	if (mouseclick.IsZero() == false)
	{
		FVector2 dir = (mouseclick - m_fPlayerCharacter->GetActorLocation()).Normalize();
		if ((mouseclick - m_fPlayerCharacter->GetActorLocation()).Length() > 0.1f)
		{
			FVector2 location = m_fPlayerCharacter->GetActorLocation();
			FVector2 updateLocation = location + dir * MoveSpeed * Time::GetElapsedTime();
			m_fPlayerCharacter->SetActorLocation(updateLocation.x, updateLocation.y);
		}
		else
		{
			Game::SetLMouseClickPosition(FVector2(0, 0));
		}
	}


	if (m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_frames)
	{
		Renderer::RenderImageWithUI(
			m_fPlayerCharacter
			->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]
			->m_frames[m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip]
			->m_frame,
			m_fPlayerCharacter,
			m_fPlayerCharacter->m_textui
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
		if (m_fPlayerCharacter->m_bAttackAnimationPlaying == false)
			m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Run;
		if (Input::IsKeyDown(VK_RIGHT))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Right;
			//m_fPlayerCharacter->AnimationBundle.animationComponent[(int)m_fPlayerCharacter->dirState][(int)m_fPlayerCharacter->animstate]->m_ianimationClip = 0;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x + RotateSpeed * Time::GetElapsedTime(), m_fPlayerCharacter->GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_LEFT))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Left;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x - RotateSpeed * Time::GetElapsedTime(), m_fPlayerCharacter->GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_DOWN))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Bottom;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x, m_fPlayerCharacter->GetActorLocation().y + RotateSpeed * Time::GetElapsedTime());
		}
		if (Input::IsKeyDown(VK_UP))
		{
			m_fPlayerCharacter->dirState = APlayerCharacter::DirState::Top;
			m_fPlayerCharacter->SetActorLocation(m_fPlayerCharacter->GetActorLocation().x, m_fPlayerCharacter->GetActorLocation().y - RotateSpeed * Time::GetElapsedTime());
		}
	}
	else
	{
		if (m_fPlayerCharacter->m_bAttackAnimationPlaying == false)
			m_fPlayerCharacter->animstate = APlayerCharacter::AnimationState::Idle;
	}
}
