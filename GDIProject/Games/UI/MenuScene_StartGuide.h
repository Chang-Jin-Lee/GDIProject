#pragma once

#include <UI/Widget.h>
#include <UI/UIButtonComponent.h>

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
	std::shared_ptr<SUIButtonComponent> m_startGameButton;
	std::shared_ptr<SUITextComponent> m_startGameButtonText;

	std::shared_ptr<SUIButtonComponent> m_endGameButton;
	std::shared_ptr<SUITextComponent> m_endGameButtonText;
};