#pragma once

#include <memory>
#include <string>
#include <Classes/Scene/Scene.h>
#include <Experiment/QuadTree.h>
#include <Time/Time.h>
#include "../Character/PlayerCharacter.h"
#include "../Character/EnemyCharacter.h"
#include "../Tile/Tile.h"
#include "../UI/PlayScene_Widget.h"
#include "../Manager/TurnManager.h"


#define TILE_COL_SIZE 20
#define TILE_ROW_SIZE 30

using FQuadTree = TQuadTree<std::shared_ptr<UObject>, 4>;
class SUITextComponent;

class UPlayScene : public UScene
{
public:
	UPlayScene();
	~UPlayScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	virtual void DeleteNullObjects() override;

	void TimeInitialize();
	void CharactersInitialize();
	void UIInitialize();
	void TileInitilize();
	void TurnManagerInitilize();

	void UpdateCollisionDetection();
	void UpdateTime();
	void UpdateInput();
	void UpdateUI();

	// delegate
	void NextTurn();
	void SpawnUnit(int type);
	void GoNextScene();

	// 턴제 게임
	bool CheckUnitActionCount();
	void CheckVictoryConditions();
	void PopUpUI(const std::wstring& str);

	void ReadyForNextStage();

public:
	std::vector<std::vector<std::shared_ptr<ATile>>> m_tiles;
	std::shared_ptr<UPlayScene_Widget> m_PlayScene_Widget;
	FVector2 MousePosition;

private:
	std::shared_ptr<APlayerCharacter> m_fPlayerCharacter;
	std::shared_ptr<AEnemyCharacter> m_fEnemyCharacter;

	FAABBBox* WorldBound = nullptr;
	FQuadTree* quadTree = nullptr;

	float m_fFPSTime = 10.0f;
	float m_fFPSLastTime = 0;
	float m_fcountOneSecond = 0;
	int m_enemyMaxSize = 15;

	std::wstring EnemyName = L"적 캐릭터";

	// 턴제 게임 관련
	int m_iMaxTurn = 30;
	//void GameManager::Update()
	//{
	//	if (bIsGameOver)
	//		return;

	//	TurnMgr->Update();
	//}
	std::shared_ptr<TurnManager> TurnMgr;	// Scene에서 Update() 해줘야함.
	
};