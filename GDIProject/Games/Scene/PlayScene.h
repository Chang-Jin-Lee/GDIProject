#pragma once

#include <Classes/Scene/Scene.h>
#include "../PlayerCharacter.h"
#include <Time/Time.h>

class UPlayScene : public UScene
{
public:
	UPlayScene();
	~UPlayScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	void Input();

	const int MoveSpeed = 200;

	float m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	float m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	float m_fcountOneSecondAnimationScene = Time::GetTotalTime();

private:
	APlayerCharacter* m_fPlayerCharacter;
};