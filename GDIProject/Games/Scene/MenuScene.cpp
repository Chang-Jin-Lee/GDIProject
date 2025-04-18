#include "MenuScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "../Games.h"
#include "PlayScene.h"

UMenuScene::UMenuScene()
{
	//m_button = new SUIButton();
	m_image = new ABackGroundImage();
}

UMenuScene::~UMenuScene()
{
	delete m_image;
	//delete m_button;
}

void UMenuScene::Initialize()
{
	//m_button->Initialize((wchar_t*)"button!", Gdiplus::Color::White, FVector2(300, 300), FVector2(100, 50));
}

void UMenuScene::Update()
{
	if (Input::IsKeyPressed(VK_5))
	{
		UScene::ChangeScene<UPlayScene>(Game::GetNextScenePtr());
	}

	if (Input::IsKeyDown(VK_R))
	{
		float rotation = m_image->GetActorRotation();
		m_image->SetActorRotation(rotation + Time::GetElapsedTime() * 20);
	}

	Renderer::RenderActor(m_image);
	//Renderer::RenderUIButton(m_button->m_Position.x, m_button->m_Position.y, m_button->m_Size.x, m_button->m_Size.y, m_button->m_pen);
}

void UMenuScene::LoadData()
{
	m_image->LoadData(L"Image", L"TitleImage.png");
	m_image->SetActorSize(1280, 800);
}

void UMenuScene::Release()
{

}
