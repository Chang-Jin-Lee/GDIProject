#pragma once

#include <Classes/GameFramework/GameState.h>

// 전역으로 게임 전체에 쓰일 변수를 저장하는 클래스
class AdGameState : public AGameStateBase
{
public:
	AdGameState();
	virtual void Initialize() override;
	virtual void Release() override;

private:
	static bool registered;

public:
	int GameScore = 0;
};