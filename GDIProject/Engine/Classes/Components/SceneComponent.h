#pragma once
#include "../../Math/Math.h"


class USceneComponent
{
public:
	FVector2 m_fsceneLocation;
	
	USceneComponent() 
	{
		m_fsceneLocation.x = 0;
		m_fsceneLocation.y = 0;
	}

	USceneComponent(float _x, float _y) 
	{
		m_fsceneLocation.x = _x;
		m_fsceneLocation.y = _y;
	}

	~USceneComponent() {};

	void Update();

	void SetSceneComponentLocation(float x, float y)
	{
		m_fsceneLocation.x = x;
		m_fsceneLocation.y = y;
	}

	FVector2 GetSceneComponentLocation() { return m_fsceneLocation; }
};