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

	Renderer::RenderActor(m_image);

}
void UEndScene::LoadData()
{
	m_image->LoadData(L"Image", L"EndImage.png");
	m_image->SetActorSize(1280, 800);
}

void UEndScene::Release()
{

}
