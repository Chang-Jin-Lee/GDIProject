#pragma once

#include <UI/Widget.h>
class SUITextComponent; // forward decl
class SUIButtonComponent; // forward decl

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
    // shake effect params
    float m_ShakeAmplitude = 3.0f;
    float m_ShakeFrequency = 25.0f;
    float m_popupTextBaseX = 0.0f;
    float m_popupTextBaseY = 0.0f;
    float m_popupRectBaseX = 0.0f;
    float m_popupRectBaseY = 0.0f;
    bool m_popupBaseCaptured = false;

public:
	std::weak_ptr<SUITextComponent> m_remainTurnGuideui;
	std::weak_ptr<SUITextComponent> m_remainTurnui;

	// 스폰 버튼
	std::weak_ptr<SUIButtonComponent> m_spawnSettelerUnitButton;
	std::weak_ptr<SUITextComponent> m_spawnSettelerUnitButtonText;
	std::weak_ptr<SUIButtonComponent> m_spawnWarriorUnitButton;
	std::weak_ptr<SUITextComponent> m_spawnWarriorUnitButtonText;
	std::weak_ptr<SUIButtonComponent> m_spawnArcherUnitButton;
	std::weak_ptr<SUITextComponent> m_spawnArcherUnitButtonText;
	std::weak_ptr<SUIButtonComponent> m_nextStageButton;
	std::weak_ptr<SUITextComponent> m_nextStageButtonText;

	std::weak_ptr<SUIButtonComponent> m_endGameButton;
	std::weak_ptr<SUITextComponent> m_endGameButtonText;

	std::weak_ptr<SUIButtonComponent> m_popupRectangle;
	std::weak_ptr<SUITextComponent> m_popupText;

	// 왼쪽아래 자세히보기 UI
	std::weak_ptr<SUIButtonComponent> m_selectTileInfomationRectangle;
	std::weak_ptr<SUITextComponent> m_selectTileName;
	std::weak_ptr<SUITextComponent> m_selectTileActionCount;

    // 유닛 턴 넘기기 버튼 (우하단)
    std::weak_ptr<SUIButtonComponent> m_skipTurnButton;
    std::weak_ptr<SUITextComponent> m_skipTurnButtonText;
};