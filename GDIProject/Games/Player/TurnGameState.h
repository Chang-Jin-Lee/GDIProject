#pragma once

#include <Classes/GameFramework/GameState.h>
#include "../Manager/TurnManager.h"

// 전역으로 게임 전체에 쓰일 변수를 저장하는 클래스
class TurnGameState : public GameStateBase
{
public:
	TurnGameState();
	virtual void Initialize() override;
	virtual void PostInitialize() override;
	virtual void Release() override;

	void CheckVictoryConditions();

private:
	static bool registered;

public:
	int m_gGameScore = 0;

	// 타일 이미지 관련
	int** m_TileCloneInfo = nullptr;
	int m_TileCloneInfoRowSize = -1;
	int m_TileCloneInfoColsize = -1;
	Gdiplus::Bitmap* m_baseTileImage;


	// 턴제 게임 관련
	int TurnCount = 0;
	int MaxTurn = 30;
	std::shared_ptr<TurnManager> TurnMgr;	// Scene에서 Update() 해줘야함.
	//void GameManager::Update()
	//{
	//	if (bIsGameOver)
	//		return;

	//	TurnMgr->Update();
	//}
	bool bIsGameOver = false;
};