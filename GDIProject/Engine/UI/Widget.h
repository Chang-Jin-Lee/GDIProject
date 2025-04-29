#pragma once

#include "../Classes/Object.h"
#include "UITextComponent.h"

// 상속받아 위젯 클래스를 만들고 그 안에 원하는 컴포넌를 추가해 위젯 만들기
class UWidget : public UObject
{
public:
	UWidget();
	~UWidget();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;


public:
	std::vector<std::shared_ptr<UWidgetComponent>> WidgetComponents;
	bool bAttached = false;
	bool bVisible = true;
};