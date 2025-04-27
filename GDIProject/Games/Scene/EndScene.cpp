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

UEndScene::UEndScene()
{
	m_image = NewObject<ABackGroundImage>(TEXT("m_image"));
	m_scoreGuide = CreateWidget<UEndScene_ScoreGuide>(TEXT("scoreGuide"));
}

UEndScene::~UEndScene()
{
	m_image.reset();
	m_scoreGuide.reset();
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
	m_image->SetActorSize(Renderer::GetResolution().x, Renderer::GetResolution().y);
}

void UEndScene::Release()
{

}

void UEndScene::DeleteNullObjects()
{
	__super::DeleteNullObjects();
}

void UEndScene::UIInitialize()
{
	m_scoreGuide->Initialize();
	TurnGameState* g = dynamic_cast<TurnGameState*>(Game::GetGameState());
	if (g)
	{
		//wchar_t gameScoreStr[10];
		//swprintf_s(gameScoreStr, 10, L"%d", g->m_gGameScore);
		//wcscpy_s(m_scoreGuide->m_scoreui->m_content, 10, gameScoreStr);
		m_scoreGuide->m_scoreui->m_content = std::to_wstring(g->m_gGameScore);
		std::cout << g->m_gGameScore << '\n';
	}
}

void UEndScene::UpdateInput()
{
	if (Input::IsKeyPressed(VK_C))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextScenePtr());
	}
}
