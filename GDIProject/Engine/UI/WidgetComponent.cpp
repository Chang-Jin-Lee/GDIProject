#include "WidgetComponent.h"
#include "../Classes/Actor.h"

void UWidgetComponent::Initialize()
{
	__super::Initialize();
}

void UWidgetComponent::Update()
{
	__super::Update();
}

void UWidgetComponent::Release()
{
	__super::Release();
}

void UWidgetComponent::AttachedUIToActor(AActor* actor)
{
	m_AttachedActor = actor;
}
