#include "Widget.h"

UWidget::UWidget()
{
	SetName(L"widget");
	//m_uiTextComponents.clear();
}

UWidget::~UWidget()
{
	for (auto WidgetComponent : WidgetComponents)
	{
		WidgetComponent.reset();
	}
	WidgetComponents.clear();
}

void UWidget::Initialize()
{
	__super::Initialize();
}

void UWidget::Update()
{
	__super::Update();
}

void UWidget::Release()
{
	__super::Release();
}
