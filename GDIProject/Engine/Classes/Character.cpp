#include "Character.h"
#include "../Runtime/Core/FIleHelper.h"
#include "../Runtime/Renderer/Renderer.h"
#include "../Time/Time.h"

ACharacter::ACharacter()
{
	SetName((wchar_t*)L"ACharacter");
}

ACharacter::~ACharacter()
{
}

void ACharacter::Initialize()
{
	__super::Initialize();

	bPlayingAnimation = false;
	// 애니메이션 실행을 위한 시간 초기화
	m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	m_fcountOneSecondAnimationScene = Time::GetTotalTime();
}

void ACharacter::Update()
{
	__super::Update();

	//Renderer::RenderCharacterAnimation(AnimationBundle, this);

	if (bPlayingAnimation)	// 플레이 가능할 때만 플레이
	{
		m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
		if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
		{
			if (AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationMaxSize != 0)
			{
				AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip =
					(AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip + 1)
					% AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationMaxSize;
			}
			m_fcountOneSecondAnimationScene = Time::GetTotalTime();
		}
	}
}

void ACharacter::Release()
{
	__super::Release();
}

void ACharacter::LoadAnimationData(const wchar_t* baseDir, const wchar_t* baseSate, const wchar_t delimeter, AnimationState animState, DirState dirState)
{
	int** cloneInfo = nullptr;
	int cloneInfoRowSize = -1;
	int cloneInfoColsize = -1;

	wchar_t originalImagefileName[MAX_INFOFILE_NAME_SIZE] = { L'\0', };
	int num = swprintf(originalImagefileName, 50, L"%s_%s.png", baseSate, GetAnimStateName(animState));
	wchar_t infoFileName[MAX_INFOFILE_NAME_SIZE] = { L'\0', };
	num = swprintf(infoFileName, MAX_INFOFILE_NAME_SIZE, L"%s_%s_%s.txt", baseSate, GetAnimStateName(animState), GetDirStateName(static_cast<DirState>(dirState)));

	FFileHelper::LoadFileToArrayWithDelimeter<int>(baseDir, infoFileName, delimeter, 100, &cloneInfoRowSize, &cloneInfoColsize, &cloneInfo);
	AnimationBundle.baseImages[static_cast<int>(animState)]->LoadData(baseDir, originalImagefileName);
	AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(animState)]->Initialize(cloneInfoRowSize, cloneInfoColsize);
	AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(animState)]->LoadData(AnimationBundle.baseImages[static_cast<int>(animState)]->mesh, cloneInfo, cloneInfoRowSize, cloneInfoColsize, PixelFormat32bppARGB);
}

const wchar_t* ACharacter::GetDirStateName(DirState state)
{
	switch (state)
	{
	case ACharacter::DirState::Left:
		return L"Left";
	case ACharacter::DirState::Top:
		return L"Top";
	case ACharacter::DirState::Right:
		return L"Right";
	case ACharacter::DirState::Bottom:
		return L"Bottom";
	default:
		return L"Unkown";
	}
}

const wchar_t* ACharacter::GetAnimStateName(AnimationState state)
{
	switch (state)
	{
	case ACharacter::AnimationState::Idle:
		return L"Idle_full";
	case ACharacter::AnimationState::Run:
		return L"Run_full";
	case ACharacter::AnimationState::Attack:
		return L"Attack_full";
	default:
		return L"Unkown";
		break;
	}
	return nullptr;
}
