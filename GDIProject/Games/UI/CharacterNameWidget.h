#pragma once

#include <UI/Widget.h>

class UCharacterNameWidget : public UWidget
{
public:
	UCharacterNameWidget();
	~UCharacterNameWidget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::shared_ptr<SUITextComponent> m_nameUI;
};