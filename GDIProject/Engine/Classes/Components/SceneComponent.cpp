#include "SceneComponent.h"
#include "../../Math/Math.h"

struct USceneComponent::Impl
{
	FVector2 sceneLocation{0, 0};
	FVector2 sceneScale{1.0f, 1.0f};
	float rotator{0.0f};
};

USceneComponent::USceneComponent()
{
	pImpl = std::make_unique<Impl>();
}

USceneComponent::USceneComponent(float _x, float _y)
{
	pImpl = std::make_unique<Impl>();
	pImpl->sceneLocation = FVector2(_x, _y);
}

USceneComponent::~USceneComponent() = default;

void USceneComponent::Update()
{
}

void USceneComponent::SetSceneComponentScale(float x, float y)
{
	pImpl->sceneScale = FVector2(x, y);
}

void USceneComponent::SetSceneComponentLocation(float x, float y)
{
	pImpl->sceneLocation = FVector2(x, y);
}

void USceneComponent::SetSceneComponentLocation(const FVector2& location)
{
	pImpl->sceneLocation = location;
}

void USceneComponent::SetSceneComponentRotation(float value)
{
	if (value > 360)
		pImpl->rotator = 0;
	else
		pImpl->rotator = value;
}

FVector2 USceneComponent::GetSceneComponentLocation() const
{
	return pImpl->sceneLocation;
}

FVector2 USceneComponent::GetSceneComponentScale() const
{
	return pImpl->sceneScale;
}

float USceneComponent::GetSceneComponentRotation() const
{
	return pImpl->rotator;
}
