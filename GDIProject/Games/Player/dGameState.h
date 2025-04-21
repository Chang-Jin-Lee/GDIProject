#pragma once

#include <Classes/GameFramework/GameState.h>

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