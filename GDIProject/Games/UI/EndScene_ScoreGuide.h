#pragma once

#include <UI/Widget.h>
#include <UI/UIButtonComponent.h>
#include <memory>

class UEndScene_Widget : public UWidget
{
public:
	UEndScene_Widget();
	~UEndScene_Widget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::shared_ptr<SUITextComponent> m_scoreui;
	std::shared_ptr<SUITextComponent> m_scoreGuideui;

	std::shared_ptr<SUIButtonComponent> m_startGameButton;
	std::shared_ptr<SUITextComponent> m_startGameButtonText;
};