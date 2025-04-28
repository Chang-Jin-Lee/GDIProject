#include "EndScene_ScoreGuide.h"
#include <Runtime/Renderer/Renderer.h>
#include "../Games.h"

UEndScene_Widget::UEndScene_Widget()
{
	m_scoreui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreui"));
	m_scoreGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreGuideui"));

	m_startGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("startGameButton"));
	m_startGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("startGameButtonText"));

	WidgetComponents.push_back(m_scoreui);
	WidgetComponents.push_back(m_scoreGuideui);
	WidgetComponents.push_back(m_startGameButton);
	WidgetComponents.push_back(m_startGameButtonText);
}

UEndScene_Widget::~UEndScene_Widget()
{
}

void UEndScene_Widget::Initialize()
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

	int x = int(Renderer::GetResolution().x * 0.2);
	int y = int(Renderer::GetResolution().y * 0.6);
	int width = 120;
	int height = 60;
	m_startGameButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
	m_startGameButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_startGameButtonText->Initialize
	(
		L"다시 시작하기",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x, y),
		FVector2(width, height)
	);
	m_startGameButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
}

void UEndScene_Widget::Update()
{
	__super::Update();
}

void UEndScene_Widget::Release()
{
	__super::Release();
}
