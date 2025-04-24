#include "EndScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "MenuScene.h"
#include "../Games.h"
#include "../Player/dGameState.h"
#include <iostream>
#include <UI/UITextComponent.h>
#include "../Image/BackGroundImage.h"

UEndScene::UEndScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_scoreui = NewObject<SUITextComponent>(TEXT("m_scoreui"));
	m_scoreGuideui = NewObject<SUITextComponent>(TEXT("m_scoreGuideui"));
}

UEndScene::~UEndScene()
{
	m_image.reset();
	m_scoreui.reset();
	m_scoreGuideui.reset();
}

void UEndScene::Initialize()
{
	__super::Initialize();
	Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(FVector2(0, 0));
	UIInitialize();
}

void UEndScene::Update()
{
	__super::Update();
	UpdateInput();
}
void UEndScene::LoadData()
{
	__super::LoadData();
	m_image->LoadData(L"Image", L"EndImage.png");
	m_image->SetActorSize(1280, 800);
}

void UEndScene::Release()
{

}

void UEndScene::UIInitialize()
{
	// 이 UI선언을 좀더 간단하게 만들어야함.
	float uiwidth = 60;
	float uiheith = 30;
	m_scoreui->Initialize
	(
		nullptr,
		18,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.4), int(Renderer::GetResolution().y * 0.5)),
		FVector2(uiwidth, uiheith)
	);
	m_scoreui->m_content = (wchar_t*)malloc(sizeof(wchar_t) * 10);
	AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (g)
	{
		wchar_t gameScoreStr[10];
		swprintf_s(gameScoreStr, 10, L"%d", g->m_gGameScore);
		wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);
		std::cout << g->m_gGameScore << '\n';
	}

	float Guideuiwidth = 250;
	float Guideuiheith = 50;
	m_scoreGuideui->Initialize
	(
		(wchar_t*)L"최종 점수 : ",
		24,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.2), int(Renderer::GetResolution().y * 0.5)),
		FVector2(Guideuiwidth, Guideuiheith)
	);
}

void UEndScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextScenePtr());
	}
}
