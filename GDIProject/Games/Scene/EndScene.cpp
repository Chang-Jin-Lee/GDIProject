#include "EndScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "MenuScene.h"
#include "../Games.h"

UEndScene::UEndScene()
{
	m_image = new ABackGroundImage();
}

UEndScene::~UEndScene()
{
	delete m_image;
}

void UEndScene::Initialize()
{

}

void UEndScene::Update()
{
	if (Input::IsKeyPressed(VK_5))
	{
		UScene::ChangeScene<UMenuScene>(Game::GetNextScenePtr());
	}

	Renderer::RenderImage(m_image->GetBitmap(), m_image->GetActorLocation().x, m_image->GetActorLocation().y, m_image->GetActorSize().x, m_image->GetActorSize().y);

}
void UEndScene::LoadData()
{
	m_image->LoadData(L"Image", L"EndImage.png");
	m_image->SetActorSize(1280, 800);
}

void UEndScene::Release()
{

}
