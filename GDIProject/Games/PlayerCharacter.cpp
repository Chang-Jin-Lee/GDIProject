#include "PlayerCharacter.h"
#include <Runtime/Core/FIleHelper.h>

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
