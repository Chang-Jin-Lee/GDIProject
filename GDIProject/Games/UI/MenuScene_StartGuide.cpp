#include "MenuScene_StartGuide.h"
#include "../Games.h"

UMenuscene_StartGuide::UMenuscene_StartGuide()
{
	m_startGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("startGameButton"));
	m_startGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("startGameButtonText"));

	m_endGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("endGameButton"));
	m_endGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("endGameButtonText"));

	WidgetComponents.push_back(m_startGameButton);
	WidgetComponents.push_back(m_startGameButtonText);
	WidgetComponents.push_back(m_endGameButton);
	WidgetComponents.push_back(m_endGameButtonText);
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
}
