#include "EndScene_ScoreGuide.h"
#include <Runtime/Renderer/Renderer.h>

UEndScene_ScoreGuide::UEndScene_ScoreGuide()
{
	m_scoreui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreui"));
	m_scoreGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreGuideui"));
	WidgetComponents.push_back(m_scoreui);
	WidgetComponents.push_back(m_scoreGuideui);
}

UEndScene_ScoreGuide::~UEndScene_ScoreGuide()
{
}

void UEndScene_ScoreGuide::Initialize()
{
	__super::Initialize();

	m_scoreui->Initialize
	(
		L"",
		18,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.4), int(Renderer::GetResolution().y * 0.5)),
		FVector2(60, 30)
	);
	m_scoreui->m_content = _wcsdup(std::to_wstring(10.0f).c_str());

	m_scoreGuideui->Initialize
	(
		L"최종 점수 : ",
		24,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.2), int(Renderer::GetResolution().y * 0.5)),
		FVector2(250, 50)
	);
}

void UEndScene_ScoreGuide::Update()
{
	__super::Update();
}

void UEndScene_ScoreGuide::Release()
{
	__super::Release();
}
