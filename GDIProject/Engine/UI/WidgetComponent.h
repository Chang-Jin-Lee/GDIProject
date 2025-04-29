#pragma once
#include "WidgetComponentBase.h"
#include "../Math/Math.h"

class AActor;

class UWidgetComponent : public UWidgetComponentBase
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void AttachedUIToActor(AActor* actor);

	bool IsVisible() { return m_bVisible; }

public:
	AActor* m_AttachedActor = nullptr;
	FVector2 m_Position;
	FVector2 m_Size;

	bool m_bVisible = true;
};