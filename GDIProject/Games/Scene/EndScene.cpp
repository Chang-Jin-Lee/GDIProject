#include "EndScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "MenuScene.h"
#include "../Games.h"
#include "../Player/dGameState.h"
#include <iostream>

UEndScene::UEndScene()
{
	m_image = new ABackGroundImage();
	m_scoreui = new SUIText();
	m_scoreGuideui = new SUIText();
}

UEndScene::~UEndScene()
{
	delete m_image;
	delete m_scoreui;
	delete m_scoreGuideui;
}

void UEndScene::Initialize()
{
	UIInitialize();
}

void UEndScene::Update()
{
	UpdateInput();
	Renderer::RenderActor(m_image);
	Renderer::RenderTextUI(m_scoreui, int(Renderer::GetResolution().x * 0.4), int(Renderer::GetResolution().y * 0.5));
	Renderer::RenderTextUI(m_scoreGuideui, int(Renderer::GetResolution().x * 0.2), int(Renderer::GetResolution().y * 0.5));

}
void UEndScene::LoadData()
{
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
		FVector2(-uiwidth / 2, -uiheith / 2),
		FVector2(uiwidth, uiheith)
	);
	m_scoreui->m_content = (wchar_t*)malloc(sizeof(wchar_t) * 10);
	AdGameState* g = dynamic_cast<AdGameState*>(Game::GetGameState());
	if (g)
	{
		wchar_t gameScoreStr[10];
		swprintf_s(gameScoreStr, 10, L"%d", g->GameScore);
		wcscpy_s(m_scoreui->m_content, 10, gameScoreStr);
		std::cout << g->GameScore << '\n';
	}

	float Guideuiwidth = 250;
	float Guideuiheith = 50;
	m_scoreGuideui->Initialize
	(
		(wchar_t*)L"최종 점수 : ",
		24,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(-Guideuiwidth / 2, -Guideuiheith / 2),
		FVector2(Guideuiwidth, Guideuiheith),
		Gdiplus::FontStyleBold,
		Gdiplus::UnitPoint,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringAlignmentNear,
		Gdiplus::StringTrimmingNone
	);
}

void UEndScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextScenePtr());
	}
}
