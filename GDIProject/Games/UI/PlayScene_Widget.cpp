#include "PlayScene_Widget.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include <Experiment/SmartCast.h>
#include <gdiplus.h>
#include <UI/UITextComponent.h>
#include <UI/UIButtonComponent.h>
#include <Classes/Camera/CameraActor.h>
#include <Runtime/Renderer/Renderer.h>
#include <Math/Math.h>

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
    if (const auto popupText = Cast<SUITextComponent>(m_popupText))
    {
        popupText->SetVisible(false);
    }
    if (const auto ref = Cast<SUIButtonComponent>(m_popupRectangle))
    {
        ref->SetVisible(false);
    }

	m_selectTileInfomationRectangle = CreateDefaultSubobject<SUIButtonComponent>(TEXT("m_selectTileInfomationRectangle"));
	m_selectTileName = CreateDefaultSubobject<SUITextComponent>(TEXT("m_selectTileName"));
	m_selectTileActionCount = CreateDefaultSubobject<SUITextComponent>(TEXT("m_selectTileActionCount"));
    m_skipTurnButton = CreateDefaultSubobject<SUIButtonComponent>(TEXT("m_skipTurnButton"));
    m_skipTurnButtonText = CreateDefaultSubobject<SUITextComponent>(TEXT("m_skipTurnButtonText"));

	WidgetComponents.push_back(m_remainTurnGuideui);
	WidgetComponents.push_back(m_remainTurnui);
	WidgetComponents.push_back(m_spawnSettelerUnitButton);
	WidgetComponents.push_back(m_spawnWarriorUnitButton);
	WidgetComponents.push_back(m_spawnArcherUnitButton);
	WidgetComponents.push_back(m_spawnSettelerUnitButtonText);
	WidgetComponents.push_back(m_spawnWarriorUnitButtonText);
	WidgetComponents.push_back(m_spawnArcherUnitButtonText);
	WidgetComponents.push_back(m_nextStageButton);
	WidgetComponents.push_back(m_nextStageButtonText);
	WidgetComponents.push_back(m_endGameButton);
	WidgetComponents.push_back(m_endGameButtonText);
	WidgetComponents.push_back(m_popupRectangle);
	WidgetComponents.push_back(m_popupText);
	WidgetComponents.push_back(m_selectTileInfomationRectangle);
	WidgetComponents.push_back(m_selectTileName);
	WidgetComponents.push_back(m_selectTileActionCount);
    WidgetComponents.push_back(m_skipTurnButton);
    WidgetComponents.push_back(m_skipTurnButtonText);
}

UPlayScene_Widget::~UPlayScene_Widget()
{
	m_remainTurnGuideui.reset();
	m_remainTurnui.reset();

	m_spawnSettelerUnitButton.reset();
	m_spawnSettelerUnitButtonText.reset();
	m_spawnWarriorUnitButton.reset();
	m_spawnWarriorUnitButtonText.reset();
	m_spawnArcherUnitButton.reset();
	m_spawnArcherUnitButtonText.reset();
	m_nextStageButton.reset();
	m_nextStageButtonText.reset();

	m_endGameButton.reset();
	m_endGameButtonText.reset();

	m_popupRectangle.reset();
	m_popupText.reset();

	m_selectTileInfomationRectangle.reset();
	m_selectTileName.reset();
	m_selectTileActionCount.reset();
}

void UPlayScene_Widget::Initialize()
{
	__super::Initialize();


	// PlayScene UI Layout (1024 x 600)
	// Top bar (y=8, h=40) : Exit[8,8,120x40] | Turn label[388,8,140x40] count[534,8,66x40] | NextTurn[870,8,146x40]
	// Spawn buttons (y=510, h=52, w=116, gap=14) : Settler x=324  Warrior x=454  Archer x=584
	// Info panel (x=8, y=456, w=210, h=110) : Name[14,462,196x28]  Action[14,494,196x28]  Skip[14,526,110x36]
	// Popup center (x=342, y=272, w=340, h=56)
	if (const auto cameraRef = Game::GetGameState()->GetMainCamera().lock())
	{
		if (const auto ref = Cast<SUITextComponent>(m_remainTurnGuideui))
		{
			ref->Initialize
			(
				L"\uD604\uC7AC \uD134 : ",
				18,
				L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(388, 8),
				FVector2(140, 40)
			);
			ref->AttachedUIToActor(cameraRef);
		}
		

		if (const auto ref = Cast<SUITextComponent>(m_remainTurnui))
		{
			if (g_TurnGameStateInstanceIsValid)
			{
				ref->Initialize
				(
					std::to_wstring(g_TurnGameStateInstance->m_iTurnCount),
					15,
					L"Verdana",
					Gdiplus::Color(255, 255, 255),
				FVector2(534, 8),
				FVector2(66, 40)
				);
			}
			else
			{
				ref->Initialize
				(
					L"",
					15,
					L"Verdana",
					Gdiplus::Color(255, 255, 255),
				FVector2(534, 8),
				FVector2(66, 40)
				);
			}
			ref->AttachedUIToActor(cameraRef);
		}

		const int BTN_RADIUS = 8;
		int x = 324;   // (1024 - 116*3 - 14*2) / 2 = 324
		int y = 510;
		int width = 116;
		int height = 52;
		if (const auto ref = Cast<SUIButtonComponent>(m_spawnSettelerUnitButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), BTN_RADIUS);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_spawnSettelerUnitButtonText))
		{
			ref->Initialize
			(
				L"\uAC1C\uCC99\uC790 \uC2A4\uD3F0\uD558\uAE30",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = 454;
		y = 510;
		if (const auto ref = Cast<SUIButtonComponent>(m_spawnWarriorUnitButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), BTN_RADIUS);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_spawnWarriorUnitButtonText))
		{
			ref->Initialize
			(
				L"\uC804\uC0AC \uC2A4\uD3F0\uD558\uAE30",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		if (const auto ref = Cast<SUIButtonComponent>(m_spawnArcherUnitButton))
		{
			x = 584;
			y = 510;
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), BTN_RADIUS);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_spawnArcherUnitButtonText))
		{
			ref->Initialize
			(
				L"\uAD81\uC218 \uC2A4\uD3F0\uD558\uAE30",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = 870;   // 1024 - 8 - 146
		y = 8;
		width = 146;
		height = 40;
		if (const auto ref = Cast<SUIButtonComponent>(m_nextStageButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 10);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_nextStageButtonText))
		{
			ref->Initialize
			(
				L"\uB2E4\uC74C \uD134",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = 8;
		y = 8;
		width = 120;
		if (const auto ref = Cast<SUIButtonComponent>(m_endGameButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), BTN_RADIUS);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_endGameButtonText))
		{
			ref->Initialize
			(
				L"\uACB0\uACFC\uD654\uBA74\uC73C\uB85C \uAC00\uAE30",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}


		x = 512;
		y = 300;
		width = 340;
		height = 56;
		if (const auto ref = Cast<SUIButtonComponent>(m_popupRectangle))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x - width / 2, y - height / 2), FVector2(width, height), BTN_RADIUS);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_popupText))
		{
			ref->Initialize
			(
				L"\uC548\uB0B4 \uD14D\uC2A4\uD2B8. \uC544\uC9C1 \uD589\uB3D9 \uC218\uAC00 \uB0A8\uC544 \uC788\uC2B5\uB2C8\uB2E4.",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x - width / 2, y - height / 2),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = 113;   // info panel center-x: left=8, w=210 -> cx=8+105=113
		y = 511;   // info panel center-y: top=456, h=110 -> cy=456+55=511
		width = 210;
		height = 110;
		if (const auto ref = Cast<SUIButtonComponent>(m_selectTileInfomationRectangle))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x - width / 2, y - height / 2), FVector2(width, height), BTN_RADIUS);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_selectTileName))
		{
			ref->Initialize
			(
				L"\uCE90\uB9AD\uD130 \uC774\uB984",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(14, 462),
				FVector2(196, 28)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		if (const auto ref = Cast<SUITextComponent>(m_selectTileActionCount))
		{
			ref->Initialize
			(
				L"\uB0A8\uC740 \uD589\uB3D9 \uC218",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(14, 494),
				FVector2(196, 28)
			);
			ref->AttachedUIToActor(cameraRef);
		}

        // Skip Turn button (우하단 info 영역 내부 오른쪽 아래)
        int sx = 69;   // skip btn: left=14, w=110 -> cx=14+55=69
        int sy = 544;  // skip btn: top=526, h=36 -> cy=526+18=544
        int sw = 110;
        int sh = 36;
        if (const auto ref = Cast<SUIButtonComponent>(m_skipTurnButton))
        {
            ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(sx - sw / 2, sy - sh / 2), FVector2(sw, sh), 8);
            ref->AttachedUIToActor(cameraRef);
        }
        if (const auto ref = Cast<SUITextComponent>(m_skipTurnButtonText))
        {
            ref->Initialize
            (
                L"\uD134 \uB118\uAE30\uAE30",
                10,
                (wchar_t*)L"Verdana",
                Gdiplus::Color(255, 255, 255),
                FVector2(sx - sw / 2, sy - sh / 2),
                FVector2(sw, sh)
            );
            ref->AttachedUIToActor(cameraRef);
        }
	}
	
	
}

void UPlayScene_Widget::Update()
{
	__super::Update();
}

void UPlayScene_Widget::Release()
{
	__super::Release();

	m_remainTurnGuideui.reset();
	m_remainTurnui.reset();
	m_spawnSettelerUnitButton.reset();
	m_spawnSettelerUnitButtonText.reset();
	m_spawnWarriorUnitButton.reset();
	m_spawnWarriorUnitButtonText.reset();
	m_spawnArcherUnitButton.reset();
	m_spawnArcherUnitButtonText.reset();
	m_nextStageButton.reset();
	m_nextStageButtonText.reset();
	m_endGameButton.reset();
	m_endGameButtonText.reset();
	m_popupRectangle.reset();
	m_popupText.reset();
	m_selectTileInfomationRectangle.reset();
	m_selectTileName.reset();
	m_selectTileActionCount.reset();
}