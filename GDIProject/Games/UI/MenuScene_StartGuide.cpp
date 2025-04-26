#include "MenuScene_StartGuide.h"
#include <Runtime/Renderer/Renderer.h>

UMenuscene_StartGuide::UMenuscene_StartGuide()
{
	m_startGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreui"));
	WidgetComponents.push_back(m_startGuideui);
}

UMenuscene_StartGuide::~UMenuscene_StartGuide()
{
}

void UMenuscene_StartGuide::Initialize()
{
	__super::Initialize();

	m_startGuideui->Initialize
	(
		(wchar_t*)L"C 키를 눌러 다음으로 넘어가세요",
		22,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.07), int(Renderer::GetResolution().y * 0.6)),
		FVector2(300, 100)
	);
}

void UMenuscene_StartGuide::Update()
{
	__super::Update();
}

void UMenuscene_StartGuide::Release()
{
	__super::Release();
}
