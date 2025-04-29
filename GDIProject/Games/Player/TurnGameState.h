#pragma once

#include <Classes/GameFramework/GameState.h>

// 전역으로 게임 전체에 쓰일 변수를 저장하는 클래스
class TurnGameState : public GameStateBase
{
public:
	TurnGameState();
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void Release() override;

private:
	static bool registered;

public:
	int m_gGameScore = 0;

	// 턴제 시스템 관련
	int m_iTurnCount = 0;	// 현재 턴
	int m_iTurnMax = 0;		// 턴 수
	bool m_bGameOver = false;
	bool m_bPlayerWin = false;

	// 타일 이미지 관련
	int** m_TileCloneInfo = nullptr;
	int m_TileCloneInfoRowSize = -1;
	int m_TileCloneInfoColsize = -1;
	Gdiplus::Bitmap* m_baseTileImage;
};
#define g_TurnGameStateInstance (dynamic_cast<TurnGameState*>(Game::GetGameState()))
#define g_TurnGameStateInstanceIsValid (g_TurnGameStateInstance != nullptr)