#pragma once

#include <UI/Widget.h>

class UEndScene_ScoreGuide : public UWidget
{
public:
	UEndScene_ScoreGuide();
	~UEndScene_ScoreGuide();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::shared_ptr<SUITextComponent> m_scoreui;
	std::shared_ptr<SUITextComponent> m_scoreGuideui;
};