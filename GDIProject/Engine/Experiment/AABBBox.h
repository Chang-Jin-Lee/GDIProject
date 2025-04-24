#pragma once
#include "../Math/Math.h"

struct FAABBBox
{
	FAABBBox() : m_minVector(0,0), m_maxVector(0,0) {}
	FAABBBox(const float& minX, const  float& minY, const float& maxX, const float& maxY) : m_minVector(minX, minY), m_maxVector(maxX, maxY) {}
	~FAABBBox() {}

	void SetMinVector(float x, float y) { m_minVector = FVector2(x, y); }
	void SetMaxVector(float x, float y) { m_maxVector = FVector2(x, y); }

	FVector2 GetMinVector() { return m_minVector; }
	FVector2 GetMaxVector() { return m_maxVector; }

	FVector2 GetCenter()
	{
		return FVector2((m_minVector + m_maxVector) * 0.5f);
	}

	FVector2 m_minVector, m_maxVector;
};