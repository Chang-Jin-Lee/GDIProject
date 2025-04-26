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

public:
	std::shared_ptr<SUITextComponent> m_remainTimeGuideui;
	std::shared_ptr<SUITextComponent> m_remainTimeui;
	std::shared_ptr<SUITextComponent> m_ButtonText;
	std::shared_ptr<SUIButtonComponent> m_Button;
};