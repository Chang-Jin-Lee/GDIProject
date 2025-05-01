#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include <Classes/Scene/Scene.h>
#include "PlayScene.h"

UMenuScene::UMenuScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_MenuSceneWidget = CreateWidget<UMenuscene_StartGuide>(TEXT("m_MenuSceneWidget"), EUILAYER::HUD);

	m_MenuSceneWidget->m_startGameButton->SetVoidDelegate([this]() { StartGame(); });
	m_MenuSceneWidget->m_endGameButton->SetVoidDelegate([this]() { EndGame(); });
}

UMenuScene::~UMenuScene()
{
	m_image.reset();
	m_MenuSceneWidget.reset();
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
	m_MenuSceneWidget->Initialize();
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

void UMenuScene::StartGame()
{
	printf("StartGame");
	UScene::ChangeScene<UPlayScene>(Game::GetNextScenePtr());
}

void UMenuScene::EndGame()
{
	printf("EndGame");
	PostQuitMessage(0);
}
