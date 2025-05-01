#pragma once

#include <UI/Widget.h>
#include <UI/UIButtonComponent.h>

class UPlayScene_Widget : public UWidget
{
public:
	UPlayScene_Widget();
	~UPlayScene_Widget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	bool m_bPopUpText = false;
	double m_currentTime = 0;
	double m_PopUpTextDelay = 1.3;

public:
	std::shared_ptr<SUITextComponent> m_remainTurnGuideui;
	std::shared_ptr<SUITextComponent> m_remainTurnui;

	// 스폰 버튼
	std::shared_ptr<SUIButtonComponent> m_spawnSettelerUnitButton;
	std::shared_ptr<SUITextComponent> m_spawnSettelerUnitButtonText;
	std::shared_ptr<SUIButtonComponent> m_spawnWarriorUnitButton;
	std::shared_ptr<SUITextComponent> m_spawnWarriorUnitButtonText;
	std::shared_ptr<SUIButtonComponent> m_spawnArcherUnitButton;
	std::shared_ptr<SUITextComponent> m_spawnArcherUnitButtonText;
	std::shared_ptr<SUIButtonComponent> m_nextStageButton;
	std::shared_ptr<SUITextComponent> m_nextStageButtonText;

	std::shared_ptr<SUIButtonComponent> m_endGameButton;
	std::shared_ptr<SUITextComponent> m_endGameButtonText;

	std::shared_ptr<SUIButtonComponent> m_popupRectangle;
	std::shared_ptr<SUITextComponent> m_popupText;

	// 왼쪽아래 자세히보기 UI
	std::shared_ptr<SUIButtonComponent> m_selectTileInfomationRectangle;
	std::shared_ptr<SUITextComponent> m_selectTileName;
	std::shared_ptr<SUITextComponent> m_selectTileActionCount;
};