#pragma once

#include <Classes/Scene/Scene.h>
#include <Experiment/QuadTree.h>
#include <Time/Time.h>
#include "../Character/PlayerCharacter.h"
#include "../Character/EnemyCharacter.h"
#include <string>
#include "../Tile/Tile.h"

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

	void TimeInitialize();
	void CharactersInitialize();
	void UIInitialize();

	void UpdateCollisionDetection();
	void UpdateTime();
	void UpdateInput();
	void UpdateUI();

private:
	std::shared_ptr<APlayerCharacter> m_fPlayerCharacter;
	std::shared_ptr<AEnemyCharacter> m_fEnemyCharacter;
	std::shared_ptr<SUITextComponent> m_scoreui;
	std::shared_ptr<SUITextComponent> m_remainTimeGuideui;
	std::shared_ptr<SUITextComponent> m_remainTimeui;
	std::shared_ptr<ATile> m_tile;

	FAABBBox* WorldBound = nullptr;
	FQuadTree* quadTree = nullptr;

	float m_fFPSTime = 10.0f;
	float m_fFPSLastTime = 0;
	float m_fcountOneSecond = 0;
	int m_enemyMaxSize = 15;

	std::wstring PlayerName = L"플레이어 캐릭터";
	std::wstring EnemyName = L"적 캐릭터";
};