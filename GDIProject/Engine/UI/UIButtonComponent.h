#pragma once
#include "WidgetComponent.h"

class SUIButtonComponent : public UWidgetComponent
{
public:
	SUIButtonComponent()
	{
		m_brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255)); // ±âº» ÇÏ¾á»ö
		m_Position = FVector2(0, 0);
		m_Size = FVector2(150, 150);
	}
	~SUIButtonComponent() {}
	
	void Initialize(Gdiplus::Color color, FVector2 Position, FVector2 Size, int _radius)
	{
		if(m_brush == nullptr) m_brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255));
		m_brush->SetColor(color);
		m_Position = Position;
		m_Size = Size;
		m_radius = _radius;
	}

public:
	Gdiplus::SolidBrush* m_brush;
	int m_radius = 5;
};