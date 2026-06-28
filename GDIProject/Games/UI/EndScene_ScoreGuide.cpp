#include "EndScene_ScoreGuide.h"
#include <Runtime/Renderer/Renderer.h>
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include <gdiplus.h>
#include <UI/UITextComponent.h>
#include <UI/UIButtonComponent.h>
#include <Classes/Camera/CameraActor.h>
#include <Math/Math.h>

UEndScene_Widget::UEndScene_Widget()
{
	m_scoreui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreui"));
	m_scoreGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("scoreGuideui"));

	m_startGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("startGameButton"));
	m_startGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("startGameButtonText"));

    if (auto sp = m_scoreui.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_scoreGuideui.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_startGameButton.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_startGameButtonText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
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
			L"\uB2E4\uC2DC \uC2DC\uC791\uD558\uAE30",
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
			L"\uD134 \uC218 : ",
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
