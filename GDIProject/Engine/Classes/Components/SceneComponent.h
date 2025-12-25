#pragma once
#include <memory>
#include "../Object.h"

template<typename T> class TVector2; using FVector2 = TVector2<float>;

class USceneComponent : public UObject
{
public:
	USceneComponent();
	USceneComponent(float _x, float _y);
	~USceneComponent();

	void Update();

	void SetSceneComponentScale(float x, float y);
	void SetSceneComponentLocation(float x, float y);
	void SetSceneComponentLocation(const FVector2& location);
	void SetSceneComponentRotation(float value);

    FVector2 GetSceneComponentLocation() const;
    FVector2 GetSceneComponentScale() const;
	float GetSceneComponentRotation() const;

private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};