#include "PlayerCharacter.h"
#include <Runtime/Core/FIleHelper.h>
#include <Runtime/Renderer/Renderer.h>
#include <Time/Time.h>
#include <iostream>
#include "Games.h"
#include <Input/Input.h>
#include "Scene/PlayScene.h"

APlayerCharacter::APlayerCharacter()
{
	for (int j = 0; j < static_cast<int>(AnimationState::Max); j++)
	{
		for (int i = 0; i < static_cast<int>(DirState::Max); i++)
		{
			AnimationBundle.animationComponent[i][j] = new UAnimationComponent();
		}
		AnimationBundle.baseImages[j] = new UStaticMeshComponent();
	}
		
	dirState = DirState::Bottom;
	animstate = AnimationState::Idle;
	m_textui = new SUIText();
}

APlayerCharacter::~APlayerCharacter()
{
	for (int j = 0; j < static_cast<int>(AnimationState::Max); j++)
	{
		for (int i = 0; i < static_cast<int>(DirState::Max); i++)
		{
			delete AnimationBundle.animationComponent[i][j];
		}
		delete AnimationBundle.baseImages[j];
	}
	delete m_textui;
}
#ifndef MAX_INFOFILE_NAME_SIZE
#define MAX_INFOFILE_NAME_SIZE 50
#endif 
void APlayerCharacter::Initialize()
{
	const wchar_t delimeter = L',';

	for (int dirState = 0; dirState < static_cast<int>(DirState::Max); dirState++)
	{
		int** cloneInfo = nullptr;
		int cloneInfoRowSize = -1;
		int cloneInfoColsize = -1;
		const wchar_t* baseDir = L"Character1/Unarmed_Idle";
		const wchar_t* baseSate = L"Unarmed";
		wchar_t originalImagefileName[MAX_INFOFILE_NAME_SIZE] = { L'\0', };
		int num = swprintf(originalImagefileName, 50, L"%s_%s.png", baseSate, GetAnimStateName(AnimationState::Idle));

		wchar_t infoFileName[MAX_INFOFILE_NAME_SIZE] = { L'\0', };
		num = swprintf(infoFileName, MAX_INFOFILE_NAME_SIZE, L"%s_%s_%s.txt", baseSate, GetAnimStateName(AnimationState::Idle), GetDirStateName(static_cast<DirState>(dirState)));

		FFileHelper::LoadFileToArrayWithDelimeter<int>(baseDir, infoFileName, delimeter, 100, &cloneInfoRowSize, &cloneInfoColsize, &cloneInfo);
		AnimationBundle.baseImages[static_cast<int>(AnimationState::Idle)]->LoadData(baseDir, originalImagefileName);
		AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(AnimationState::Idle)]->Initialize(cloneInfoRowSize, cloneInfoColsize);
		AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(AnimationState::Idle)]->LoadData(AnimationBundle.baseImages[static_cast<int>(AnimationState::Idle)]->mesh, cloneInfo, cloneInfoRowSize, cloneInfoColsize, PixelFormat32bppARGB);

		baseDir = L"Character1/Unarmed_Run";
		baseSate = L"Unarmed";
		memset(originalImagefileName, L'\0', MAX_INFOFILE_NAME_SIZE);
		num = swprintf(originalImagefileName, 50, L"%s_%s.png", baseSate, GetAnimStateName(AnimationState::Run));

		memset(infoFileName, L'\0', MAX_INFOFILE_NAME_SIZE);
		num = swprintf(infoFileName, MAX_INFOFILE_NAME_SIZE, L"%s_%s_%s.txt", baseSate, GetAnimStateName(AnimationState::Run), GetDirStateName(static_cast<DirState>(dirState)));
		FFileHelper::LoadFileToArrayWithDelimeter<int>(baseDir, infoFileName, delimeter, 100, &cloneInfoRowSize, &cloneInfoColsize, &cloneInfo);
		AnimationBundle.baseImages[static_cast<int>(AnimationState::Run)]->LoadData(baseDir, originalImagefileName);
		AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(AnimationState::Run)]->Initialize(cloneInfoRowSize, cloneInfoColsize);
		AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(AnimationState::Run)]->LoadData(AnimationBundle.baseImages[static_cast<int>(AnimationState::Run)]->mesh, cloneInfo, cloneInfoRowSize, cloneInfoColsize, PixelFormat32bppARGB);


		baseDir = L"Character1/Sword_Attack";
		baseSate = L"Sword";
		memset(originalImagefileName, L'\0', MAX_INFOFILE_NAME_SIZE);
		num = swprintf(originalImagefileName, 50, L"%s_%s.png", baseSate, GetAnimStateName(AnimationState::Attack));

		memset(infoFileName, L'\0', MAX_INFOFILE_NAME_SIZE);
		num = swprintf(infoFileName, MAX_INFOFILE_NAME_SIZE, L"%s_%s_%s.txt", baseSate, GetAnimStateName(AnimationState::Attack), GetDirStateName(static_cast<DirState>(dirState)));
		FFileHelper::LoadFileToArrayWithDelimeter<int>(baseDir, infoFileName, delimeter, 100, &cloneInfoRowSize, &cloneInfoColsize, &cloneInfo);
		AnimationBundle.baseImages[static_cast<int>(AnimationState::Attack)]->LoadData(baseDir, originalImagefileName);
		AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(AnimationState::Attack)]->Initialize(cloneInfoRowSize, cloneInfoColsize);
		AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(AnimationState::Attack)]->LoadData(AnimationBundle.baseImages[static_cast<int>(AnimationState::Attack)]->mesh, cloneInfo, cloneInfoRowSize, cloneInfoColsize, PixelFormat32bppARGB);
	}

	// 시간 초기화
	m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();

	m_fcharacterRotationSpeed = 360;

	// SceneComponent 값 초기화
	SetActorLocation(50, 50);
	SetActorScale(2.5f, 2.5f);
	SetName((wchar_t*)L"플레이어 캐릭터");
	// UI 초기화
	int uiwidth = 120;
	int uiheith = 20;
	m_textui->Initialize
	(
		GetName(),
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(-50, -20),
		FVector2(uiwidth, uiheith),
		Gdiplus::FontStyleBold,
		Gdiplus::UnitPoint,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringTrimmingNone
	);
}

void APlayerCharacter::Update()
{
	FVector2 mouseclick = Game::GetLMouseClickPosition();

	std::cout << mouseclick.x << ' ' << mouseclick.y << '\n';
	std::cout << GetActorLocation().x << ' ' << GetActorLocation().y << '\n';

	if (mouseclick.IsZero() == false)
	{
		FVector2 dir = (mouseclick - GetActorLocation()).Normalize();
		if ((mouseclick - GetActorLocation()).Length() > 0.1f)
		{
			FVector2 location = GetActorLocation();
			FVector2 updateLocation = location + dir * MoveSpeed * Time::GetElapsedTime();
			SetActorLocation(updateLocation.x, updateLocation.y);
		}
		else
		{
			Game::SetLMouseClickPosition(FVector2(0, 0));
		}
	}


	if (AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_frames)
	{
		Renderer::RenderImageWithUI(
			AnimationBundle.animationComponent[(int)dirState][(int)animstate]
			->m_frames[AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip]
			->m_frame,
			this,
			m_textui
		);
	}

	m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
	if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
	{
		if (AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationMaxSize != 0)
		{
			if (AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip == AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationMaxSize - 1)
			{
				if (animstate == APlayerCharacter::AnimationState::Attack)
				{
					m_bAttackAnimationPlaying = false;
				}
			}
			AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip =
				(AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip + 1)
				% AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationMaxSize;
		}
		m_fcountOneSecondAnimationScene = Time::GetTotalTime();
	}
}

void APlayerCharacter::Release()
{

}

void APlayerCharacter::Input()
{

	if (Input::IsKeyDown(VK_R))
	{
		float rotation = GetActorRotation();
		SetActorRotation(rotation + Time::GetElapsedTime() * m_fcharacterRotationSpeed);
	}

	if (Input::IsKeyDown(VK_SPACE))
	{
		if (m_bAttackAnimationPlaying == false)
		{
			AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip = 0;
			m_bAttackAnimationPlaying = true;
			animstate = APlayerCharacter::AnimationState::Attack;
		}
	}

	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_DOWN))
	{
		if (m_bAttackAnimationPlaying == false)
			animstate = APlayerCharacter::AnimationState::Run;
		if (Input::IsKeyDown(VK_RIGHT))
		{
			dirState = APlayerCharacter::DirState::Right;
			//AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip = 0;
			SetActorLocation(GetActorLocation().x + RotateSpeed * Time::GetElapsedTime(), GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_LEFT))
		{
			dirState = APlayerCharacter::DirState::Left;
			SetActorLocation(GetActorLocation().x - RotateSpeed * Time::GetElapsedTime(), GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_DOWN))
		{
			dirState = APlayerCharacter::DirState::Bottom;
			SetActorLocation(GetActorLocation().x, GetActorLocation().y + RotateSpeed * Time::GetElapsedTime());
		}
		if (Input::IsKeyDown(VK_UP))
		{
			dirState = APlayerCharacter::DirState::Top;
			SetActorLocation(GetActorLocation().x, GetActorLocation().y - RotateSpeed * Time::GetElapsedTime());
		}
	}
	else
	{
		if (m_bAttackAnimationPlaying == false)
			animstate = APlayerCharacter::AnimationState::Idle;
	}
}

void APlayerCharacter::LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat)
{
	AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(animState)]->LoadData(baseImage, cloneInfo, rowSize, colSize, pixelformat);
}

void APlayerCharacter::SetAnimMeshScale(float width, float height)
{

}

const wchar_t* APlayerCharacter::GetDirStateName(DirState state)
{
	switch (state)
	{
	case APlayerCharacter::DirState::Left:
		return L"Left";
	case APlayerCharacter::DirState::Top:
		return L"Top";
	case APlayerCharacter::DirState::Right:
		return L"Right";
	case APlayerCharacter::DirState::Bottom:
		return L"Bottom";
	default:
		return L"Unkown";
	}
}

const wchar_t* APlayerCharacter::GetAnimStateName(AnimationState state)
{
	switch (state)
	{
	case APlayerCharacter::AnimationState::Idle:
		return L"Idle_full";
	case APlayerCharacter::AnimationState::Run:
		return L"Run_full";
	case APlayerCharacter::AnimationState::Attack:
		return L"Attack_full";
	default:
		return L"Unkown";
		break;
	}
	return nullptr;
}
