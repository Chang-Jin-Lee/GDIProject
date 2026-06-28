#include "MenuScene_StartGuide.h"
#include "../Games.h"
#include <Runtime/Core/FIleHelper.h>
#include <gdiplus.h>
#include <UI/UITextComponent.h>
#include <UI/UIButtonComponent.h>
#include <Classes/Camera/CameraActor.h>
#include <Runtime/Renderer/Renderer.h>
#include <Math/Math.h>

UMenuscene_StartGuide::UMenuscene_StartGuide()
{
	m_startGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("startGameButton"));
	m_startGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("startGameButtonText"));
	m_connectGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("connectGameButton"));
	m_connectGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("connectGameButtonText"));
	m_hostGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("hostGameButton"));
	m_hostGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("hostGameButtonText"));
	m_ipInputText = CreateDefaultSubobject<SUITextComponent>(TEXT("ipInputText"));
	m_statusText = CreateDefaultSubobject<SUITextComponent>(TEXT("statusText"));

	m_endGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("endGameButton"));
	m_endGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("endGameButtonText"));

	HallOfFameText = CreateDefaultSubobject<SUITextComponent>(TEXT("HallOfFameText"));

    if (auto sp = m_startGameButton.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_startGameButtonText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_connectGameButton.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_connectGameButtonText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_hostGameButton.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_hostGameButtonText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_ipInputText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_statusText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_endGameButton.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = m_endGameButtonText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
    if (auto sp = HallOfFameText.lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));

	for (int i = 0; i < 10; i++)
	{
        textScore.emplace_back(CreateDefaultSubobject<SUITextComponent>(TEXT("textScore") + std::to_wstring(i)));
        textTime.emplace_back(CreateDefaultSubobject<SUITextComponent>(TEXT("textTime") + std::to_wstring(i)));
        if (auto sp = textScore.back().lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
        if (auto sp = textTime.back().lock()) WidgetComponents.push_back(std::static_pointer_cast<UWidgetComponent>(sp));
	}
}

UMenuscene_StartGuide::~UMenuscene_StartGuide()
{
	m_startGameButton.reset();
	m_startGameButtonText.reset();
	m_endGameButton.reset();
	m_endGameButtonText.reset();
}

void UMenuscene_StartGuide::Initialize()
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
			L"\uc2f1\uae00\ud50c\ub808\uc774\ud558\uae30",
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

	x = int(Renderer::GetResolution().x * 0.2);
	y = int(Renderer::GetResolution().y * 0.52);
	width = 180;
	height = 52;
	if (auto btn = Cast<SUIButtonComponent>(m_connectGameButton))
	{
		btn->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera())) btn->AttachedUIToActor(cameraRef);
	}
	if (auto text = Cast<SUITextComponent>(m_connectGameButtonText))
	{
		text->Initialize(L"\uba40\ud2f0\ud50c\ub808\uc774\uc811\uc18d\ud558\uae30", 10, (wchar_t*)L"Verdana", Gdiplus::Color(255, 255, 255), FVector2(x, y), FVector2(width, height));
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera())) text->AttachedUIToActor(cameraRef);
	}

	x = int(Renderer::GetResolution().x * 0.2);
	y = int(Renderer::GetResolution().y * 0.64);
	if (auto btn = Cast<SUIButtonComponent>(m_hostGameButton))
	{
		btn->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera())) btn->AttachedUIToActor(cameraRef);
	}
	if (auto text = Cast<SUITextComponent>(m_hostGameButtonText))
	{
		text->Initialize(L"\uba40\ud2f0\ud50c\ub808\uc774\ubc29\ub9cc\ub4e4\uae30", 10, (wchar_t*)L"Verdana", Gdiplus::Color(255, 255, 255), FVector2(x, y), FVector2(width, height));
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera())) text->AttachedUIToActor(cameraRef);
	}

	if (auto text = Cast<SUITextComponent>(m_ipInputText))
	{
		text->Initialize(L"IP: 127.0.0.1", 10, (wchar_t*)L"Verdana", Gdiplus::Color(240, 240, 240), FVector2(int(Renderer::GetResolution().x * 0.2), int(Renderer::GetResolution().y * 0.74)), FVector2(220, 34));
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera())) text->AttachedUIToActor(cameraRef);
	}
	if (auto text = Cast<SUITextComponent>(m_statusText))
	{
		text->Initialize(L"", 10, (wchar_t*)L"Verdana", Gdiplus::Color(240, 240, 120), FVector2(int(Renderer::GetResolution().x * 0.2), int(Renderer::GetResolution().y * 0.79)), FVector2(260, 34));
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera())) text->AttachedUIToActor(cameraRef);
	}
	x = int(Renderer::GetResolution().x * 0.2);
	y = int(Renderer::GetResolution().y * 0.86);
	width = 120;
	height = 60;
	if (auto btn = Cast<SUIButtonComponent>(m_endGameButton))
	{
		btn->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
		if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
		{
			btn->AttachedUIToActor(cameraRef);
		}
	}
	if (auto text = Cast<SUITextComponent>(m_endGameButtonText))
	{
		text->Initialize
		(
			L"\uac8c\uc784 \uc885\ub8cc",
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

	x = int(Renderer::GetResolution().x * 0.65);
	y = int(Renderer::GetResolution().y * 0.15);
	width = 240;
	height = 60;
	if (auto text = Cast<SUITextComponent>(HallOfFameText))
	{
		text->Initialize
		(
			L"\uBA85\uC608\uC758 \uC804\uB2F9",
			12,
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

	std::vector<FFileHelper::ScoreEntry> scoreList;
	FFileHelper::LoadFileString(L"/highscore.txt", scoreList);
	width = 240;
	height = 60;
	for (int i = 0; i < scoreList.size(); i++)
	{
		x = int(Renderer::GetResolution().x * 0.5);
		y = int(Renderer::GetResolution().y * 0.25) + int((Renderer::GetResolution().y * 0.07) * i);

		if (auto text = Cast<SUITextComponent>(textScore[i]))
		{
			text->Initialize
			(
				std::to_wstring(scoreList[i].score),
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

		if (auto text = Cast<SUITextComponent>(textTime[i]))
		{
			text->Initialize
			(
				scoreList[i].time,
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x + int(Renderer::GetResolution().x * 0.2), y),
				FVector2(width, height)
			);
			if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
			{
				text->AttachedUIToActor(cameraRef);
			}
		}
	}
}

void UMenuscene_StartGuide::Update()
{
	__super::Update();
}

void UMenuscene_StartGuide::Release()
{
	__super::Release();
	m_startGameButton.reset();
	m_startGameButtonText.reset();
	m_endGameButton.reset();
	m_endGameButtonText.reset();
	textScore.clear();
	textTime.clear();
}
