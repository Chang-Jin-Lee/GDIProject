#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include "PlayScene.h"

UMenuScene::UMenuScene()
{
	m_image = new ABackGroundImage();
	m_startGuideui = new SUIText();
}

UMenuScene::~UMenuScene()
{
	delete m_image;
	delete m_startGuideui;
}

void UMenuScene::Initialize()
{
	UIInitialize();
}

void UMenuScene::Update()
{
	UpdateInput();

	Renderer::RenderActor(m_image);
	Renderer::RenderTextUI(m_startGuideui, Renderer::GetResolution().x * 0.2, Renderer::GetResolution().y * 0.6);
}

void UMenuScene::LoadData()
{
	m_image->LoadData(L"Image", L"TitleImage.png");
	m_image->SetActorSize(1280, 800);
}

void UMenuScene::Release()
{

}

void UMenuScene::UIInitialize()
{
	int uiwidth = 250;
	int uiheith = 100;
	m_startGuideui->Initialize
	(
		(wchar_t*)L"C 키를 눌러 다음으로 넘어가세요",
		22,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(-uiwidth / 2, -uiheith / 2),
		FVector2(uiwidth, uiheith)
	);
}

void UMenuScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UPlayScene>(Game::GetNextScenePtr());
	}

	if (Input::IsKeyDown(VK_R))
	{
		float rotation = m_image->GetActorRotation();
		m_image->SetActorRotation(rotation + Time::GetElapsedTime() * 20);
	}
}
