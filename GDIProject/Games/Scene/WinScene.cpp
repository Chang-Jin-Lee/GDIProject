#include "WinScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "MenuScene.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include <iostream>
#include <UI/UITextComponent.h>
#include "../Image/BackGroundImage.h"
#include "../UI/EndScene_ScoreGuide.h"
#include <Runtime/Core/FIleHelper.h>

UWinScene::UWinScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_Widget = CreateWidget<UEndScene_Widget>(TEXT("scoreGuide"), EUILAYER::HUD);

	if (auto widget = Cast<UEndScene_Widget>(m_Widget))
	{
		if (auto btn = Cast<SUIButtonComponent>(widget->m_startGameButton))
		{
			btn->SetVoidDelegate([this]() { StartGame(); });
		}
	}
}

UWinScene::~UWinScene()
{
	m_image.reset();
	m_Widget.reset();
}

void UWinScene::Initialize()
{
	__super::Initialize();
	if (auto cameraRef = Cast<ACameraActor>(Game::GetGameState()->GetMainCamera()))
	{
		cameraRef->SetCameraLocation(FVector2(0, 0));
	}
	UIInitialize();
}

void UWinScene::Update()
{
	__super::Update();
	UpdateInput();
}
void UWinScene::LoadData()
{
	__super::LoadData();
	if (auto imageRef = Cast<ABackGroundImage>(m_image))
	{
		imageRef->LoadData(L"Image", L"WinImage.png");
		imageRef->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
	}
}

void UWinScene::Release()
{
	__super::Release();
}

void UWinScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UWinScene::UIInitialize()
{
	if (auto widget = Cast<UEndScene_Widget>(m_Widget))
	{
		widget->Initialize();
		if (g_TurnGameStateInstanceIsValid)
		{
			if (auto scoreui = Cast<SUITextComponent>(widget->m_scoreui))
			{
				scoreui->m_content = std::to_wstring(g_TurnGameStateInstance->m_iTurnCount);
			}
			FFileHelper::SaveScoreAndKeepTop10(L"/highscore.txt", g_TurnGameStateInstance->m_iTurnCount);
			std::cout << g_TurnGameStateInstance->m_iTurnCount << '\n';
		}
	}
	
}

void UWinScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
	}
}

void UWinScene::StartGame()
{
	UScene::ChangeScene<UMenuScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
}
