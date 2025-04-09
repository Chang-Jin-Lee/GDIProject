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
	Renderer::RenderImage(m_image->GetBitmap(), m_image->GetActorLocation().x, m_image->GetActorLocation().y, m_image->GetActorSize().x, m_image->GetActorSize().y);
}

void UMenuScene::LoadData()
{
	m_image->LoadData(L"Image", L"TitleImage.png");
	m_image->SetActorSize(1280, 800);
}

void UMenuScene::Release()
{

}
