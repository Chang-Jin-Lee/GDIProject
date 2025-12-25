#pragma once

#include "../Classes/Object.h"

class UWidgetComponentBase : public UObject
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
};