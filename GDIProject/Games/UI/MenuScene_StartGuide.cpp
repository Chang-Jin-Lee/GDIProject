#include "MenuScene_StartGuide.h"
#include "../Games.h"
#include <Runtime/Core/FIleHelper.h>

UMenuscene_StartGuide::UMenuscene_StartGuide()
{
	m_startGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("startGameButton"));
	m_startGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("startGameButtonText"));

	m_endGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("endGameButton"));
	m_endGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("endGameButtonText"));

	HallOfFameText = CreateDefaultSubobject<SUITextComponent>(TEXT("HallOfFameText"));

	WidgetComponents.push_back(m_startGameButton);
	WidgetComponents.push_back(m_startGameButtonText);
	WidgetComponents.push_back(m_endGameButton);
	WidgetComponents.push_back(m_endGameButtonText);
	WidgetComponents.push_back(HallOfFameText);

	for (int i = 0; i < 10; i++)
	{
		textScore.emplace_back(CreateDefaultSubobject<SUITextComponent>(TEXT("textScore") + std::to_wstring(i)));
		textTime.emplace_back(CreateDefaultSubobject<SUITextComponent>(TEXT("textTime") + std::to_wstring(i)));
		WidgetComponents.push_back(textScore.back());
		WidgetComponents.push_back(textTime.back());
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
			L"게임 시작",
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
	y = int(Renderer::GetResolution().y * 0.8);
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
			L"게임 종료",
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
			L"명예의 전당",
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
