#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include <Classes/Scene/Scene.h>
#include "PlayScene.h"
#include <Experiment/SmartCast.h>
#include <Runtime/Core/FIleHelper.h>
#include <UI/UIButtonComponent.h>

UMenuScene::UMenuScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_MenuSceneWidget = CreateWidget<UMenuscene_StartGuide>(TEXT("m_MenuSceneWidget"), EUILAYER::HUD);

	if (const auto widgetRef = Cast<UMenuscene_StartGuide>(m_MenuSceneWidget))
	{
		if (auto btn = Cast<SUIButtonComponent>(widgetRef->m_startGameButton))
		{
			btn->SetVoidDelegate([this]() { StartGame(); });
		}
		if (auto btn = Cast<SUIButtonComponent>(widgetRef->m_endGameButton))
		{
			btn->SetVoidDelegate([this]() { EndGame(); });
		}
	}
}

UMenuScene::~UMenuScene()
{
	m_image.reset();
	m_MenuSceneWidget.reset();
}

void UMenuScene::Initialize()
{
	__super::Initialize();
	if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
	{
		cameraRef->SetCameraLocation(FVector2(0, 0));
	}
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
	if (auto imageRef = Cast<ABackGroundImage>(m_image))
	{
		imageRef->LoadData(L"Image", L"TitleImage.png");
		imageRef->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
	}
}

void UMenuScene::Release()
{
	__super::Release();
	m_image.reset();
	m_MenuSceneWidget.reset();
}

void UMenuScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UMenuScene::UIInitialize()
{
	if (auto widgetRef = Cast<UMenuscene_StartGuide>(m_MenuSceneWidget))
	{
		widgetRef->Initialize();
	}
}

void UMenuScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UPlayScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
	}

	if (Input::IsKeyDown(VK_R))
	{
		if (auto imageRef = Cast<ABackGroundImage>(m_image))
		{
			const float& rotation = imageRef->GetActorRotation();
			imageRef->SetActorRotation(rotation + Time::GetElapsedTime() * 20);
		}
	}
}

void UMenuScene::StartGame()
{
	UScene::ChangeScene<UPlayScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
}

void UMenuScene::EndGame()
{
	PostQuitMessage(0);
}
