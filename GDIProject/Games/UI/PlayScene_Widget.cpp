#include "PlayScene_Widget.h"
#include "../Games.h"

UPlayScene_Widget::UPlayScene_Widget()
{
	m_remainTimeGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("remainTimeGuideui"));
	m_remainTimeui = CreateDefaultSubobject<SUITextComponent>(TEXT("remainTimeui"));
	m_spawnSettelerUnitButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("spawnSettelerUnitButton"));
	m_spawnWarriorUnitButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("spawnWarriorUnitButton"));
	m_spawnArcherUnitButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("spawnArcherUnitButton"));
	m_spawnSettelerUnitButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("m_spawnSettelerUnitButtonText"));
	m_spawnWarriorUnitButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("m_spawnWarriorUnitButtonText"));
	m_spawnArcherUnitButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("m_spawnArcherUnitButtonText"));
	m_nextStageButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("nextStageButton"));
	m_nextStageButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("nextStageButtonText"));
	m_endGameButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("endGameButton"));
	m_endGameButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("endGameButtonText"));
	m_popupText = CreateDefaultSubobject<SUITextComponent>(TEXT("m_popupText"));
	m_popupText->m_bVisible = false;

	WidgetComponents.push_back(m_remainTimeGuideui);
	WidgetComponents.push_back(m_remainTimeui);
	WidgetComponents.push_back(m_spawnSettelerUnitButton);
	WidgetComponents.push_back(m_spawnWarriorUnitButton);
	WidgetComponents.push_back(m_spawnArcherUnitButton);
	WidgetComponents.push_back(m_spawnSettelerUnitButtonText);
	WidgetComponents.push_back(m_spawnWarriorUnitButtonText);
	WidgetComponents.push_back(m_spawnArcherUnitButtonText);
	WidgetComponents.push_back(m_spawnSettelerUnitButtonText);
	WidgetComponents.push_back(m_nextStageButton);
	WidgetComponents.push_back(m_nextStageButtonText);
	WidgetComponents.push_back(m_endGameButton);
	WidgetComponents.push_back(m_endGameButtonText);
	WidgetComponents.push_back(m_popupText);
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

	int x = int(Renderer::GetResolution().x * 0.2);
	int y = int(Renderer::GetResolution().y * 0.8);
	int width = 120;
	int height = 60;
	m_spawnSettelerUnitButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
	m_spawnSettelerUnitButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_spawnSettelerUnitButtonText->Initialize
	(
		L"개척자 스폰하기",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x, y),
		FVector2(width, height)
	);
	m_spawnSettelerUnitButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	x = int(Renderer::GetResolution().x * 0.5);
	y = int(Renderer::GetResolution().y * 0.8);
	m_spawnWarriorUnitButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
	m_spawnWarriorUnitButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_spawnWarriorUnitButtonText->Initialize
	(
		L"전사 스폰하기",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x, y),
		FVector2(width, height)
	);
	m_spawnWarriorUnitButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	x = int(Renderer::GetResolution().x * 0.8);
	y = int(Renderer::GetResolution().y * 0.8);
	m_spawnArcherUnitButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
	m_spawnArcherUnitButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_spawnArcherUnitButtonText->Initialize
	(
		L"궁수 스폰하기",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x, y),
		FVector2(width, height)
	);
	m_spawnArcherUnitButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	m_nextStageButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(int(Renderer::GetResolution().x * 0.8), int(Renderer::GetResolution().y * 0.5)), FVector2(120, 60), 10);
	m_nextStageButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_nextStageButtonText->Initialize
	(
		L"다음 턴",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.8), int(Renderer::GetResolution().y * 0.5)),
		FVector2(120, 60)
	);
	m_nextStageButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	x = int(Renderer::GetResolution().x * 0.1);
	y = int(Renderer::GetResolution().y * 0.1);
	width = 160;
	m_endGameButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
	m_endGameButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_endGameButtonText->Initialize
	(
		L"결과화면으로 가기",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x, y),
		FVector2(width, height)
	);
	m_endGameButtonText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	m_popupText->Initialize
	(
		L"안내 텍스트. 아직 행동 수가 남아 있습니다.",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.5) - 160, int(Renderer::GetResolution().y * 0.5) - 30),
		FVector2(320, 60)
	);
	m_popupText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
}

void UPlayScene_Widget::Update()
{
	__super::Update();
}

void UPlayScene_Widget::Release()
{
	__super::Release();
}
