#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>
#include <UI/UIText.h>

class APlayerCharacter : public ACharacter
{
public:
	APlayerCharacter();
	~APlayerCharacter();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void Input();

	enum class DirState
	{
		Left = 0, Top, Right, Bottom, Max
	};
	enum class AnimationState
	{
		Idle = 0, Run, Attack, Max
	};
	struct FAnimationBundle // 방향, 애니메이션
	{
		UAnimationComponent* animationComponent[static_cast<int>(DirState::Max)][static_cast<int>(AnimationState::Max)];
		UStaticMeshComponent* baseImages[static_cast<int>(AnimationState::Max)];
	};

	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat);

	void SetAnimMeshScale(float width, float height);
	const wchar_t* GetDirStateName(DirState state);
	const wchar_t* GetAnimStateName(AnimationState state);

	FAnimationBundle AnimationBundle;
	AnimationState animstate = AnimationState::Idle;
	DirState dirState = DirState::Bottom;
	bool m_bAttackAnimationPlaying = false;
	SUIText* m_textui;

	const int RotateSpeed = 200;
	const float MoveSpeed = 300;
	float m_fcharacterRotationSpeed = 85;


	float m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	float m_fFPSLastTimeAnimationScene = 0;
	float m_fcountOneSecondAnimationScene = 0;
};