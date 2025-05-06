#include "EndScene_ScoreGuide.h"
#include <Runtime/Renderer/Renderer.h>
#include "../Games.h"
#include "../Player/TurnGameState.h"

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
	m_scoreui.reset();
	m_scoreGuideui.reset();
	m_startGameButton.reset();
	m_startGameButtonText.reset();
}

void UEndScene_Widget::Initialize()
{
	__super::Initialize();

	int x = int(Renderer::GetResolution().x * 0.2);
	int y = int(Renderer::GetResolution().y * 0.6);
	int width = 120;
	int height = 60;
	if (auto btn = Cast<SUIButtonComponent>(m_startGameButton))
	{
		btn->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
		{
			btn->AttachedUIToActor(cameraRef);
		}
	}
	if (auto text = Cast<SUITextComponent>(m_startGameButtonText))
	{
		text->Initialize
		(
			L"다시 시작하기",
			10,
			(wchar_t*)L"Verdana",
			Gdiplus::Color(255, 255, 255),
			FVector2(x, y),
			FVector2(width, height)
		);
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
		{
			text->AttachedUIToActor(cameraRef);
		}
	}

	x = int(Renderer::GetResolution().x * 0.4);
	y = int(Renderer::GetResolution().y * 0.6);
	width = 250;
	height = 50;
	if (auto text = Cast<SUITextComponent>(m_scoreGuideui))
	{
		text->Initialize
		(
			L"턴 수 : ",
			24,
			(wchar_t*)L"Verdana",
			Gdiplus::Color(255, 255, 255),
			FVector2(x, y),
			FVector2(width, height)
		);
	}

	x = int(Renderer::GetResolution().x * 0.65);
	y = int(Renderer::GetResolution().y * 0.6);
	width = 60;
	height = 50;
	std::wstring content = L"";
	if (g_TurnGameStateInstanceIsValid)
	{
		content = std::to_wstring(g_TurnGameStateInstance->m_iTurnCount);
	}

	if (auto text = Cast<SUITextComponent>(m_scoreui))
	{
		text->Initialize
		(
			content,
			18,
			(wchar_t*)L"Verdana",
			Gdiplus::Color(255, 255, 255),
			FVector2(x, y),
			FVector2(width, height)
		);
		text->m_content = _wcsdup(std::to_wstring(10.0f).c_str());
	}
}

void UEndScene_Widget::Update()
{
	__super::Update();
}

void UEndScene_Widget::Release()
{
	__super::Release();
	m_scoreui.reset();
	m_scoreGuideui.reset();
	m_startGameButton.reset();
	m_startGameButtonText.reset();
}
