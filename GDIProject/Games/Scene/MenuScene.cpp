#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include "PlayScene.h"

UMenuScene::UMenuScene()
{
}

UMenuScene::~UMenuScene()
{
	delete m_image;
}

void UMenuScene::Initialize()
{
	m_image = new ABackGroundImage();
}

void UMenuScene::Update()
{
	if (Input::IsKeyPressed(VK_5))
	{
		UScene::ChangeScene<UPlayScene>(Game::GetNextScenePtr());
		//UScene::ChangeScene<UPlayScene>(Game::GetNextScene());
	}
	if (Input::IsKeyDown(VK_R))
	{
		float rotation = m_image->GetActorRotation();
		m_image->SetActorRotation(rotation + Time::GetElapsedTime() * 20);
	}

	Renderer::RenderActor(m_image);
}

void UMenuScene::LoadData()
{
	m_image->LoadData(L"Image", L"TitleImage.png");
	m_image->SetActorSize(1280, 800);
}

void UMenuScene::Release()
{

}
