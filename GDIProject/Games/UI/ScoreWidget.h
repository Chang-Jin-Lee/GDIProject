#pragma once

#include <UI/Widget.h>

class UScoreWidget : public UWidget
{
public:
	UScoreWidget();
	~UScoreWidget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::shared_ptr<SUITextComponent> m_scoreui;
};