#pragma once
#include "../../Math/Math.h"


class USceneComponent
{
public:
	FVector2 m_fsceneLocation;
	FVector2 m_fsceneScale;
	
	USceneComponent() 
	{
		m_fsceneLocation.x = 0;
		m_fsceneLocation.y = 0;
		m_fsceneScale.x = 1.0f;
		m_fsceneScale.y = 1.0f;
	}

	USceneComponent(float _x, float _y) 
	{
		m_fsceneLocation.x = _x;
		m_fsceneLocation.y = _y;
		m_fsceneScale.x = 1.0f;
		m_fsceneScale.y = 1.0f;
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

	FVector2 GetSceneComponentLocation() { return m_fsceneLocation; }
	FVector2 GetSceneComponentScale() { return m_fsceneScale; }
};