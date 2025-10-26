#pragma once

#include <UI/Widget.h>
class SUITextComponent; // forward decl
class SUIButtonComponent; // forward decl
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
	std::weak_ptr<SUITextComponent> m_scoreui;
	std::weak_ptr<SUITextComponent> m_scoreGuideui;

	std::weak_ptr<SUIButtonComponent> m_startGameButton;
	std::weak_ptr<SUITextComponent> m_startGameButtonText;
};