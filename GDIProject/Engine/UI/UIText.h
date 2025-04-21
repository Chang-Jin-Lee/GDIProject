#pragma once
#include "../Math/Math.h"

class SUIText
{
public:
	SUIText() 
	{
		m_content = (wchar_t*)L"Default_Text";
		m_fontSize = 10;
		m_fontFamily = (wchar_t*)L"Verdana";
		m_color = Gdiplus::Color(255, 255, 255);
		m_efontStyle = FontStyleBold;
		m_eworldUnit = UnitPoint;
		m_fontAlignment = StringAlignmentNear;
		m_fontLineAlignment = StringAlignmentNear;
		m_fontTrimming = StringTrimmingNone;
		m_Position = FVector2(0,0);
		m_Size = FVector2(150,150);
	}
	~SUIText() {}

		void Initialize(
			wchar_t* content,
			float fontSize,
			wchar_t* fontFamily,
			Gdiplus::Color color,
			FVector2 Position,
			FVector2 Size,
			Gdiplus::FontStyle fontStyle = Gdiplus::FontStyleBold,
			Gdiplus::Unit worldUnit = Gdiplus::UnitPoint,
			Gdiplus::StringAlignment fontAlignment = Gdiplus::StringAlignmentNear,
			Gdiplus::StringAlignment fontLineAlignment = Gdiplus::StringAlignmentNear,
			Gdiplus::StringTrimming fontTrimming = Gdiplus::StringTrimmingNone
	)
	{
		m_content = content;
		m_fontSize = fontSize;
		m_fontFamily = fontFamily;
		m_color = color;
		m_efontStyle = fontStyle;
		m_eworldUnit = worldUnit;
		m_fontAlignment = fontAlignment;// 영
		m_fontLineAlignment = fontLineAlignment;
		m_fontTrimming = fontTrimming;
		m_Position = Position;
		m_Size = Size;
	}
	
public:
	wchar_t* m_content;
	float m_fontSize;
	wchar_t* m_fontFamily;
	Gdiplus::Color m_color;
	Gdiplus::FontStyle m_efontStyle;
	Gdiplus::Unit m_eworldUnit;
	Gdiplus::StringAlignment m_fontAlignment;// 영역의 자체의 정렬
	Gdiplus::StringAlignment m_fontLineAlignment; // 다음 줄로 갱신될 때 정렬
	Gdiplus::StringTrimming m_fontTrimming;
	FVector2 m_Position;
	FVector2 m_Size;
};