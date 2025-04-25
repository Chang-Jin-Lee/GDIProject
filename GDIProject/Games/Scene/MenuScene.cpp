#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include "PlayScene.h"
#include <UI/UIButtonComponent.h>
#include <UI/UITextComponent.h>

UMenuScene::UMenuScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_startGuideui = NewObject<SUITextComponent>(TEXT("m_startGuideui"));
}

UMenuScene::~UMenuScene()
{
	m_image.reset();
	m_startGuideui.reset();
}

void UMenuScene::Initialize()
{
	__super::Initialize();
	Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(FVector2(0,0));
	UIInitialize();
}

void UMenuScene::Update()
{
	__super::Update();
	UpdateInput();
}

void UMenuScene::LoadData()
{
	__super::LoadData();
	m_image->LoadData(L"Image", L"TitleImage.png");
	m_image->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
}

void UMenuScene::Release()
{

}

void UMenuScene::UIInitialize()
{
	float uiwidth = 300;
	float uiheith = 100;
	m_startGuideui->Initialize
	(
		(wchar_t*)L"C 키를 눌러 다음으로 넘어가세요",
		22,
		(wchar_t*)L"Verdana",
		Gdiplus::Color(255, 255, 255),
		FVector2(int(Renderer::GetResolution().x * 0.07), int(Renderer::GetResolution().y * 0.6)),
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
