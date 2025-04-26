#include "PlayScene_Widget.h"
#include "../Games.h"

UPlayScene_Widget::UPlayScene_Widget()
{
	m_remainTimeGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("remainTimeGuideui"));
	m_remainTimeui = CreateDefaultSubobject<SUITextComponent>(TEXT("remainTimeui"));
	m_Button = CreateDefaultSubobject<SUIButtonComponent>(TEXT("Button"));
	m_ButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("ButtonText"));
	WidgetComponents.push_back(m_remainTimeGuideui);
	WidgetComponents.push_back(m_remainTimeui);
	WidgetComponents.push_back(m_Button);
	WidgetComponents.push_back(m_ButtonText);
}

UPlayScene_Widget::~UPlayScene_Widget()
{
}

void UPlayScene_Widget::Initialize()
{
	__super::Initialize();
	
	m_remainTimeGuideui->Initialize
	(
		L"남은 시간 : ",
		18,
		L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.35), int(Renderer::GetResolution().y * 0.15)),
		FVector2(150, 40)
	);
	m_remainTimeGuideui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	
	m_remainTimeui->Initialize
	(
		L"",
		10,
		L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.15)),
		FVector2(60, 30)
	);
	m_remainTimeui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_remainTimeui->m_content = _wcsdup(std::to_wstring(10.0f).c_str());

	m_Button->Initialize(Gdiplus::Color(48, 50, 113), FVector2(int(Renderer::GetResolution().x * 0.8), int(Renderer::GetResolution().y * 0.45)), FVector2(60, 30), 5);
	m_Button->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_ButtonText->Initialize
	(
		L"",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.8), int(Renderer::GetResolution().y * 0.45)),
		FVector2(60, 30)
	);
	m_ButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_ButtonText->m_content = _wcsdup(std::to_wstring(10.0f).c_str());
}

void UPlayScene_Widget::Update()
{
	__super::Update();
}

void UPlayScene_Widget::Release()
{
	__super::Release();
}
