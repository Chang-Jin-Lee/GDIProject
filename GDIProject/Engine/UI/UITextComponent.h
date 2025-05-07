#pragma once
#include "../Classes/Actor.h"
#include "../Experiment/StateOption.h"
#include "WidgetComponent.h"

class SUITextComponent : public UWidgetComponent
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	SUITextComponent()
	{
		m_content = L"";
		m_fontSize = 10;
		m_fontFamily = L"Verdana";
		m_color = Gdiplus::Color(255, 255, 255);
		m_efontStyle = FontStyleBold;
		m_eworldUnit = UnitPoint;
		//m_fontAlignment = StringAlignmentNear;	// 기본이 왼쪽에 붙은 정렬
		//m_fontLineAlignment = StringAlignmentNear;
		m_fontAlignment = StringAlignmentCenter;	// 기본이 중앙 정렬
		m_fontLineAlignment = StringAlignmentCenter;
		m_fontTrimming = StringTrimmingNone;
		m_Size = FVector2(150, 150);
		m_Position = FVector2(0, 0);
	}
	~SUITextComponent() {}

	void Initialize(
		std::wstring content,
		float fontSize,
		std::wstring fontFamily,
		Gdiplus::Color color,
		FVector2 Position,
		FVector2 Size,
		Gdiplus::FontStyle fontStyle = Gdiplus::FontStyleBold,
		Gdiplus::Unit worldUnit = Gdiplus::UnitPoint,
		//Gdiplus::StringAlignment fontAlignment = Gdiplus::StringAlignmentNear,
		//Gdiplus::StringAlignment fontLineAlignment = Gdiplus::StringAlignmentNear,
		Gdiplus::StringAlignment fontAlignment = Gdiplus::StringAlignmentCenter,
		Gdiplus::StringAlignment fontLineAlignment = Gdiplus::StringAlignmentCenter,
		Gdiplus::StringTrimming fontTrimming = Gdiplus::StringTrimmingNone
	)
	{
		m_content = content;
		m_fontSize = fontSize;
		m_fontFamily = fontFamily;
		m_color = color;
		m_efontStyle = fontStyle;
		m_eworldUnit = worldUnit;
		m_fontAlignment = fontAlignment;
		m_fontLineAlignment = fontLineAlignment;
		m_fontTrimming = fontTrimming;
		m_Position = Position;
		m_Size = Size;
	}

public:
	std::wstring m_content;
	float m_fontSize;
	std::wstring m_fontFamily;
	Gdiplus::Color m_color;
	Gdiplus::FontStyle m_efontStyle;
	Gdiplus::Unit m_eworldUnit;
	Gdiplus::StringAlignment m_fontAlignment;// 영역의 자체의 정렬
	Gdiplus::StringAlignment m_fontLineAlignment; // 다음 줄로 갱신될 때 정렬
	Gdiplus::StringTrimming m_fontTrimming;
	
};