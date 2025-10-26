#pragma once

#include <UI/Widget.h>
class SUITextComponent; // forward decl

class UCharacterNameWidget : public UWidget
{
public:
	UCharacterNameWidget();
	~UCharacterNameWidget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

public:
	std::weak_ptr<SUITextComponent> m_nameUI;
};