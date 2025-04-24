#pragma once
#include "../../Math/Math.h"
#include "../Object.h"

class USceneComponent : public UObject
{
public:
	FVector2 m_fsceneLocation;
	FVector2 m_fsceneScale;
	float m_fRotator;	// 0 ~ 360µµ
	
	USceneComponent() 
	{
		m_fsceneLocation = FVector2(0, 0);
		m_fsceneScale = FVector2(1.0f, 1.0f);
		m_fRotator = 0.0f;
	}

	USceneComponent(float _x, float _y) 
	{
		m_fsceneLocation = FVector2(_x,_y);
		m_fsceneScale = FVector2(1.0f, 1.0f);
		m_fRotator = 0.0f;
	}

	~USceneComponent() {};

	void Update();

	void SetSceneComponentScale(float x, float y)
	{
		m_fsceneScale = FVector2(x, y);
	}

	void SetSceneComponentLocation(float x, float y)
	{
		m_fsceneLocation = FVector2(x, y);
	}

	void SetSceneComponentLocation(const FVector2& location)
	{
		m_fsceneLocation = location;
	}

	void SetSceneComponentRotation(float value)
	{
		if (value > 360)
			m_fRotator = 0;
		else
			m_fRotator = value;
	}

	FVector2 GetSceneComponentLocation() { return m_fsceneLocation; }
	FVector2 GetSceneComponentScale() { return m_fsceneScale; }
	float GetSceneComponentRotation() { return m_fRotator; }

};