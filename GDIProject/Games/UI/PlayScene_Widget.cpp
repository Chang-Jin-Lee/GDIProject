#include "PlayScene_Widget.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"

UPlayScene_Widget::UPlayScene_Widget()
{
	m_remainTurnGuideui = CreateDefaultSubobject<SUITextComponent>(TEXT("remainTimeGuideui"));
	m_remainTurnui = CreateDefaultSubobject<SUITextComponent>(TEXT("remainTimeui"));
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
	m_popupRectangle = CreateDefaultSubobject<SUIButtonComponent>(TEXT("m_popupRectangle"));
	m_popupText = CreateDefaultSubobject<SUITextComponent>(TEXT("m_popupText"));
	m_popupText->m_bVisible = false;
	m_popupRectangle->m_bVisible = false;

	m_selectTileInfomationRectangle = CreateDefaultSubobject<SUIButtonComponent>(TEXT("m_selectTileInfomationRectangle"));
	m_selectTileName = CreateDefaultSubobject<SUITextComponent>(TEXT("m_selectTileName"));
	m_selectTileActionCount = CreateDefaultSubobject<SUITextComponent>(TEXT("m_selectTileActionCount"));

	WidgetComponents.push_back(m_remainTurnGuideui);
	WidgetComponents.push_back(m_remainTurnui);
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
	WidgetComponents.push_back(m_popupRectangle);
	WidgetComponents.push_back(m_popupText);
	WidgetComponents.push_back(m_selectTileInfomationRectangle);
	WidgetComponents.push_back(m_selectTileName);
	WidgetComponents.push_back(m_selectTileActionCount);
}

UPlayScene_Widget::~UPlayScene_Widget()
{
}

void UPlayScene_Widget::Initialize()
{
	__super::Initialize();
	
	m_remainTurnGuideui->Initialize
	(
		L"현재 턴 : ",
		18,
		L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.35), int(Renderer::GetResolution().y * 0.15)),
		FVector2(150, 40)
	);
	m_remainTurnGuideui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	
	if (g_TurnGameStateInstanceIsValid)
	{
		m_remainTurnui->Initialize
		(
			std::to_wstring(g_TurnGameStateInstance->m_iTurnCount),
			10,
			L"Verdana",
			Gdiplus::Color(255, 255, 255),
			FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.15)),
			FVector2(60, 30)
		);
	}
	else
	{
		m_remainTurnui->Initialize
		(
			L"",
			10,
			L"Verdana",
			Gdiplus::Color(255, 255, 255),
			FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.15)),
			FVector2(60, 30)
		);
		m_remainTurnui->m_content = _wcsdup(std::to_wstring(10.0f).c_str());
	}
	m_remainTurnui->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	

	int x = int(Renderer::GetResolution().x * 0.4);
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

	x = int(Renderer::GetResolution().x * 0.6);
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

	x = int(Renderer::GetResolution().x * 0.85);
	y = int(Renderer::GetResolution().y * 0.4);
	width = int(Renderer::GetResolution().x * 0.1);
	height = int(Renderer::GetResolution().y * 0.1);
	m_nextStageButton->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 10);
	m_nextStageButton->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_nextStageButtonText->Initialize
	(
		L"다음 턴",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x,y),
		FVector2(width, height)
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


	x = int(Renderer::GetResolution().x * 0.5);
	y = int(Renderer::GetResolution().y * 0.5);
	width = 320;
	height = 60;
	m_popupRectangle->Initialize(Gdiplus::Color(180, 180, 13), FVector2(x - width / 2, y - height / 2), FVector2(width, height), 10);
	m_popupRectangle->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_popupText->Initialize
	(
		L"안내 텍스트. 아직 행동 수가 남아 있습니다.",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x - width / 2, y - height /2 ),
		FVector2(width, height)
	);
	m_popupText->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	x = int(Renderer::GetResolution().x * 0.15);
	y = int(Renderer::GetResolution().y * 0.85);
	width = int(Renderer::GetResolution().x * 0.25);
	height = int(Renderer::GetResolution().y * 0.25);
	m_selectTileInfomationRectangle->Initialize(Gdiplus::Color(180, 180, 13), FVector2(x - width / 2, y - height / 2), FVector2(width, height), 10);
	m_selectTileInfomationRectangle->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
	m_selectTileName->Initialize
	(
		L"캐릭터 이름",
		10,	
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x - width / 2, y - (height * 2) / 3),
		FVector2(width, height)
	);
	m_selectTileName->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());

	m_selectTileActionCount->Initialize
	(
		L"남은 행동 수",
		10,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(x - width / 2, y - height / 3),
		FVector2(width, height)
	);
	m_selectTileActionCount->AttachedUIToActor(Game::GetGameState()->GetMainCamera().get());
}

void UPlayScene_Widget::Update()
{
	__super::Update();
}

void UPlayScene_Widget::Release()
{
	__super::Release();
}
