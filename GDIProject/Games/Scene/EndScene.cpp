#include "EndScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "MenuScene.h"
#include "../Games.h"
#include "../Player/TurnGameState.h"
#include <iostream>
#include <UI/UITextComponent.h>
#include "../Image/BackGroundImage.h"
#include "../UI/EndScene_ScoreGuide.h"
#include <Experiment/SmartCast.h>
#include <Runtime/Core/FIleHelper.h>
#include <UI/UIButtonComponent.h>

UEndScene::UEndScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_Widget = CreateWidget<UEndScene_Widget>(TEXT("scoreGuide"), EUILAYER::HUD);

	if (const auto m_WidgetRef = Cast<UEndScene_Widget>(m_Widget))
	{
		if (auto btn = Cast<SUIButtonComponent>(m_WidgetRef->m_startGameButton))
		{
			btn->SetVoidDelegate([this]() { StartGame(); });
		}
	}
}

UEndScene::~UEndScene()
{
	m_image.reset();
	m_Widget.reset();
}

void UEndScene::Initialize()
{
	__super::Initialize();
	if (const auto cameraRef = Game::GetGameState()->GetMainCamera().lock())
	{
		cameraRef->SetCameraLocation(FVector2(0, 0));
	}
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
	if (const auto imageRef = m_image.lock())
	{
		imageRef->LoadData(L"Image", L"EndImage.png");
		imageRef->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
	}
}

void UEndScene::Release()
{
	__super::Release();
	m_image.reset();
	m_Widget.reset();
}

void UEndScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UEndScene::UIInitialize()
{
	if (const auto widghtRef = m_Widget.lock())
	{
		widghtRef->Initialize();

		if (g_TurnGameStateInstanceIsValid)
		{
			if (auto text = Cast<SUITextComponent>(widghtRef->m_scoreui))
			{
                text->SetContent(std::to_wstring(g_TurnGameStateInstance->m_iTurnCount));
			}
		}
	}
}

void UEndScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
	}
}

void UEndScene::StartGame()
{
	UScene::ChangeScene<UMenuScene>(Game::GetNextSceneSharedPtr(), Game::GetNextSceneWeakPtr());
}
