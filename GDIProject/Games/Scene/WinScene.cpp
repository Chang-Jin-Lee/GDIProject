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

UWinScene::UWinScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_Widget = CreateWidget<UEndScene_Widget>(TEXT("scoreGuide"), EUILAYER::BUTTON);

	m_Widget->m_startGameButton->SetVoidDelegate([this]() { StartGame(); });
}

UWinScene::~UWinScene()
{
	m_image.reset();
	m_Widget.reset();
}

void UWinScene::Initialize()
{
	__super::Initialize();
	Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(FVector2(0, 0));
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
	m_image->LoadData(L"Image", L"WinImage.png");
	m_image->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
}

void UWinScene::Release()
{

}

void UWinScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UWinScene::UIInitialize()
{
	m_Widget->Initialize();
	if (g_TurnGameStateInstanceIsValid)
	{
		m_Widget->m_scoreui->m_content = std::to_wstring(g_TurnGameStateInstance->m_gGameScore);
		std::cout << g_TurnGameStateInstance->m_gGameScore << '\n';
	}
}

void UWinScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextScenePtr());
	}
}

void UWinScene::StartGame()
{
	printf("Go UMenuScene");
	UScene::ChangeScene<UMenuScene>(Game::GetNextScenePtr());
}
