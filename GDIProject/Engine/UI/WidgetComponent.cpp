#include "WidgetComponent.h"
#include "../Classes/Actor.h"
#include "../Math/Math.h"
#include <memory>

struct UWidgetComponent::Impl {
    FVector2 position{0, 0};
    FVector2 size{150, 150};
    bool visible{true};
    WidgetRenderType type{ WidgetRenderType::Camera };
};

UWidgetComponent::~UWidgetComponent() = default;

void UWidgetComponent::ImplDeleter::operator()(Impl* p) noexcept {
    delete p;
}

void UWidgetComponent::Initialize()
{
    if (!pImpl) pImpl.reset(new Impl());
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

bool UWidgetComponent::IsVisible() const { return pImpl && pImpl->visible; }
void UWidgetComponent::SetVisible(bool visible) { if (!pImpl) pImpl.reset(new Impl()); pImpl->visible = visible; }
void UWidgetComponent::SetPosition(const FVector2& position) { if (!pImpl) pImpl.reset(new Impl()); pImpl->position = position; }
void UWidgetComponent::SetSize(const FVector2& size) { if (!pImpl) pImpl.reset(new Impl()); pImpl->size = size; }
FVector2 UWidgetComponent::GetPosition() const { return pImpl ? pImpl->position : FVector2(0, 0); }
FVector2 UWidgetComponent::GetSize() const { return pImpl ? pImpl->size : FVector2(0, 0); }

void UWidgetComponent::SetWidgetRenderType(WidgetRenderType type) { if (!pImpl) pImpl.reset(new Impl()); pImpl->type = type; }
UWidgetComponent::WidgetRenderType UWidgetComponent::GetWidgetRenderType() const { return pImpl ? pImpl->type : WidgetRenderType::Camera; }
