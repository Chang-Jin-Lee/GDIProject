#pragma once

#include <Classes/Scene/Scene.h>
#include <Experiment/QuadTree.h>
#include <UI/UIText.h>
#include <Time/Time.h>
#include "../Character/PlayerCharacter.h"
#include "../Character/EnemyCharacter.h"

using FQuadTree = TQuadTree<std::shared_ptr<UObject>, 4>;

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

private:
	std::shared_ptr<APlayerCharacter> m_fPlayerCharacter;
	std::shared_ptr<AEnemyCharacter> m_fEnemyCharacter;

	SUIText* m_scoreui;
	SUIText* m_remainTimeGuideui;
	SUIText* m_remainTimeui;

	FAABBBox* WorldBound = nullptr;
	FQuadTree* quadTree = nullptr;

	float m_fFPSTime = 10.0f;
	float m_fFPSLastTime = 0;
	float m_fcountOneSecond = 0;
	int m_enemyMaxSize = 15;
};