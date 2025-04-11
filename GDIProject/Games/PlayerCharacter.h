#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>
#include <UI/UIText.h>

class APlayerCharacter : public ACharacter
{
public:
	APlayerCharacter();
	~APlayerCharacter();

	typedef enum class DirState
	{
		Left = 0, Top, Right, Bottom, Max
	}DirState;

	typedef enum class AnimationState
	{
		Idle = 0, Run, Attack, Max
	}AnimationState;

	struct FAnimationBundle // 방향, 애니메이션
	{
		UAnimationComponent* animationComponent[static_cast<int>(DirState::Max)][static_cast<int>(AnimationState::Max)];
		UStaticMeshComponent* baseImages[static_cast<int>(AnimationState::Max)];
	};

	FAnimationBundle AnimationBundle;

	bool m_bAttackAnimationPlaying = false;

	AnimationState animstate = AnimationState::Idle;
	DirState dirState = DirState::Bottom;
	void Initialize();
	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat);

	void SetAnimMeshScale(float width, float height);

	const wchar_t* GetDirStateName(DirState state);
	const wchar_t* GetAnimStateName(AnimationState state);

	SUIText* m_textui;
};