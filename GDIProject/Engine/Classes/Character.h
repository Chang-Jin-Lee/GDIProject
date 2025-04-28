#pragma once

#include "Pawn.h"
#include "../Runtime/Animation/AnimationComponent.h"

#ifndef MAX_INFOFILE_NAME_SIZE
#define MAX_INFOFILE_NAME_SIZE 50
#endif 

// 애니메이션을 가지는 Pawn을 Character로 정의
// 기본적으로 상하좌우에 대한 각각의 애니메이션이 존재
class ACharacter : public APawn
{
public:
	ACharacter();
	~ACharacter();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	enum class DirState
	{
		Left = 0, Top, Right, Bottom, Max
	};

	enum class AnimationState
	{
		Idle = 0, Run, Attack, Max
	};

	virtual void LoadAnimationData(const wchar_t* baseDir, const wchar_t* baseSate, const wchar_t delimeter, AnimationState animState, DirState dirState);

	struct FAnimationBundle // 방향, 애니메이션
	{
		UAnimationComponent* animationComponent[static_cast<int>(DirState::Max)][static_cast<int>(AnimationState::Max)];
		UStaticMeshComponent* baseImages[static_cast<int>(AnimationState::Max)];
	};

public:
	const wchar_t* GetDirStateName(DirState state);
	const wchar_t* GetAnimStateName(AnimationState state);
	FAnimationBundle AnimationBundle;
	AnimationState animstate = AnimationState::Idle;
	DirState dirState = DirState::Bottom;

	bool bPlayingAnimation = false;

	float m_fFPSTimeAnimationScene = 1.0f / 18.0f;
	float m_fFPSLastTimeAnimationScene = 0;
	float m_fcountOneSecondAnimationScene = 0;
};