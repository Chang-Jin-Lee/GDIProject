#pragma once
#include "../../Math/Math.h"


class USceneComponent
{
public:
	FVector2 m_fsceneLocation;
	FVector2 m_fsceneScale;
	float m_fRotator;	// 0 ~ 360µµ
	
	USceneComponent() 
	{
		m_fsceneLocation.x = 0;
		m_fsceneLocation.y = 0;
		m_fsceneScale.x = 1.0f;
		m_fsceneScale.y = 1.0f;
		m_fRotator = 0.0f;
	}

	USceneComponent(float _x, float _y) 
	{
		m_fsceneLocation.x = _x;
		m_fsceneLocation.y = _y;
		m_fsceneScale.x = 1.0f;
		m_fsceneScale.y = 1.0f;
		m_fRotator = 0.0f;
	}

	~USceneComponent() {};

	void Update();

	void SetSceneComponentScale(float x, float y)
	{
		m_fsceneScale.x = x;
		m_fsceneScale.y = y;
	}

	void SetSceneComponentLocation(float x, float y)
	{
		m_fsceneLocation.x = x;
		m_fsceneLocation.y = y;
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