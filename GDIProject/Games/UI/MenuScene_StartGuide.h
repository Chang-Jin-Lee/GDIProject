#pragma once

#include <UI/Widget.h>

class UMenuscene_StartGuide : public UWidget
{
public:
	UMenuscene_StartGuide();
	~UMenuscene_StartGuide();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::shared_ptr<SUITextComponent> m_startGuideui;
};