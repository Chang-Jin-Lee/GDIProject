#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include "PlayScene.h"
#include <UI/UIButtonComponent.h>
#include <UI/UITextComponent.h>
#include <Classes/Scene/Scene.h>

UMenuScene::UMenuScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_startGuideui = CreateWidget<UMenuscene_StartGuide>(TEXT("startGuideui"));
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

void UMenuScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UMenuScene::UIInitialize()
{
	m_startGuideui->Initialize();
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
