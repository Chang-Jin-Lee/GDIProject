#pragma once

#include "../Classes/Object.h"

class UWidget : public UObject
{
	UWidget();
	~UWidget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
};