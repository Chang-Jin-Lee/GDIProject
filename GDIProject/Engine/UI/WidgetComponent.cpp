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

void UWidgetComponent::AttachedUIToActor(const std::weak_ptr<UObject>& object)
{
	if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object.lock()))
	{
		m_AttachedActor = actor;
	}
}
