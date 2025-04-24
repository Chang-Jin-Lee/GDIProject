#pragma once

#include <Classes/GameFramework/GameState.h>

// 전역으로 게임 전체에 쓰일 변수를 저장하는 클래스
class AdGameState : public AGameStateBase
{
public:
	AdGameState();
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void Release() override;

private:
	static bool registered;

public:
	int m_gGameScore = 0;

	// 타일 이미지 관련
	int** m_TileCloneInfo = nullptr;
	int m_TileCloneInfoRowSize = -1;
	int m_TileCloneInfoColsize = -1;
	Gdiplus::Bitmap* m_baseTileImage;
};