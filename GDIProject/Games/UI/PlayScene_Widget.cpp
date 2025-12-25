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

	if (const auto cameraRef = Game::GetGameState()->GetMainCamera().lock())
	{
		if (const auto ref = Cast<SUITextComponent>(m_remainTurnGuideui))
		{
			ref->Initialize
			(
				L"현재 턴 : ",
				18,
				L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(int(Renderer::GetResolution().x * 0.35), int(Renderer::GetResolution().y * 0.15)),
				FVector2(150, 40)
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
					10,
					L"Verdana",
					Gdiplus::Color(255, 255, 255),
					FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.15)),
					FVector2(60, 30)
				);
			}
			else
			{
				ref->Initialize
				(
					L"",
					10,
					L"Verdana",
					Gdiplus::Color(255, 255, 255),
					FVector2(int(Renderer::GetResolution().x * 0.5), int(Renderer::GetResolution().y * 0.15)),
					FVector2(60, 30)
				);
                ref->SetContent(std::to_wstring(10.0f));
			}
			ref->AttachedUIToActor(cameraRef);
		}


		int x = int(Renderer::GetResolution().x * 0.4);
		int y = int(Renderer::GetResolution().y * 0.8);
		int width = 120;
		int height = 60;
		if (const auto ref = Cast<SUIButtonComponent>(m_spawnSettelerUnitButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_spawnSettelerUnitButtonText))
		{
			ref->Initialize
			(
				L"개척자 스폰하기",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = int(Renderer::GetResolution().x * 0.6);
		y = int(Renderer::GetResolution().y * 0.8);
		if (const auto ref = Cast<SUIButtonComponent>(m_spawnWarriorUnitButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_spawnWarriorUnitButtonText))
		{
			ref->Initialize
			(
				L"전사 스폰하기",
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
			x = int(Renderer::GetResolution().x * 0.8);
			y = int(Renderer::GetResolution().y * 0.8);
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_spawnArcherUnitButtonText))
		{
			ref->Initialize
			(
				L"궁수 스폰하기",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = int(Renderer::GetResolution().x * 0.85);
		y = int(Renderer::GetResolution().y * 0.4);
		width = int(Renderer::GetResolution().x * 0.1);
		height = int(Renderer::GetResolution().y * 0.1);
		if (const auto ref = Cast<SUIButtonComponent>(m_nextStageButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 10);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_nextStageButtonText))
		{
			ref->Initialize
			(
				L"다음 턴",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = int(Renderer::GetResolution().x * 0.1);
		y = int(Renderer::GetResolution().y * 0.1);
		width = 160;
		if (const auto ref = Cast<SUIButtonComponent>(m_endGameButton))
		{
			ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(x, y), FVector2(width, height), 5);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_endGameButtonText))
		{
			ref->Initialize
			(
				L"결과화면으로 가기",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x, y),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}


		x = int(Renderer::GetResolution().x * 0.5);
		y = int(Renderer::GetResolution().y * 0.5);
		width = 320;
		height = 60;
		if (const auto ref = Cast<SUIButtonComponent>(m_popupRectangle))
		{
			ref->Initialize(Gdiplus::Color(180, 180, 13), FVector2(x - width / 2, y - height / 2), FVector2(width, height), 10);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_popupText))
		{
			ref->Initialize
			(
				L"안내 텍스트. 아직 행동 수가 남아 있습니다.",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x - width / 2, y - height / 2),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		x = int(Renderer::GetResolution().x * 0.15);
		y = int(Renderer::GetResolution().y * 0.85);
		width = int(Renderer::GetResolution().x * 0.25);
		height = int(Renderer::GetResolution().y * 0.25);
		if (const auto ref = Cast<SUIButtonComponent>(m_selectTileInfomationRectangle))
		{
			ref->Initialize(Gdiplus::Color(180, 180, 13), FVector2(x - width / 2, y - height / 2), FVector2(width, height), 10);
			ref->AttachedUIToActor(cameraRef);
		}
		if (const auto ref = Cast<SUITextComponent>(m_selectTileName))
		{
			ref->Initialize
			(
				L"캐릭터 이름",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x - width / 2, y - (height * 2) / 3),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

		if (const auto ref = Cast<SUITextComponent>(m_selectTileActionCount))
		{
			ref->Initialize
			(
				L"남은 행동 수",
				10,
				(wchar_t*)L"Verdana",
				Gdiplus::Color(255, 255, 255),
				FVector2(x - width / 2, y - height / 3),
				FVector2(width, height)
			);
			ref->AttachedUIToActor(cameraRef);
		}

        // Skip Turn button (우하단 info 영역 내부 오른쪽 아래)
        int sx = int(Renderer::GetResolution().x * 0.25) + 10;
        int sy = int(Renderer::GetResolution().y * 0.92);
        int sw = 110;
        int sh = 40;
        if (const auto ref = Cast<SUIButtonComponent>(m_skipTurnButton))
        {
            ref->Initialize(Gdiplus::Color(48, 50, 113), FVector2(sx - sw / 2, sy - sh / 2), FVector2(sw, sh), 8);
            ref->AttachedUIToActor(cameraRef);
        }
        if (const auto ref = Cast<SUITextComponent>(m_skipTurnButtonText))
        {
            ref->Initialize
            (
                L"턴 넘기기",
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
