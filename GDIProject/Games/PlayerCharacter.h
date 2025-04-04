#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>

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
		UStaticMeshComponent* baseImages[static_cast<int>(DirState::Max)][static_cast<int>(AnimationState::Max)];
	};

	AnimationState animationstate = AnimationState::Idle;
	void Initialize(int rowSize, int colSize);
	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, AnimationState state, int pixelformat);

	FAnimationBundle AnimationBundle;
};