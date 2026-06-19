#pragma once

#include <UI/Widget.h>

class SUIButtonComponent;
class SUITextComponent;

class UMenuscene_StartGuide : public UWidget
{
public:
	UMenuscene_StartGuide();
	~UMenuscene_StartGuide();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::weak_ptr<SUIButtonComponent> m_startGameButton;
	std::weak_ptr<SUITextComponent> m_startGameButtonText;

	std::weak_ptr<SUIButtonComponent> m_connectGameButton;
	std::weak_ptr<SUITextComponent> m_connectGameButtonText;
	std::weak_ptr<SUIButtonComponent> m_hostGameButton;
	std::weak_ptr<SUITextComponent> m_hostGameButtonText;
	std::weak_ptr<SUITextComponent> m_ipInputText;
	std::weak_ptr<SUITextComponent> m_statusText;

	std::weak_ptr<SUIButtonComponent> m_endGameButton;
	std::weak_ptr<SUITextComponent> m_endGameButtonText;

	std::vector<std::weak_ptr<SUITextComponent>> textScore;
	std::vector<std::weak_ptr<SUITextComponent>> textTime;
	std::weak_ptr<SUITextComponent> HallOfFameText;
};