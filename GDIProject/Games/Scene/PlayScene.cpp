#include "PlayScene.h"
#include <Runtime/Renderer/Renderer.h>
#include <Input/Input.h>
#include "EndScene.h"
#include "../Games.h"
#include <iostream>

UPlayScene::UPlayScene()
{

}

UPlayScene::~UPlayScene()
{
	delete m_fPlayerCharacter;
}

void UPlayScene::Initialize()
{
	m_fPlayerCharacter = new APlayerCharacter();
	m_fPlayerCharacter->Initialize();
}

void UPlayScene::Update()
{
	m_fPlayerCharacter->Update();

	Input();
}

void UPlayScene::LoadData()
{
	
}

void UPlayScene::Release()
{

}

void UPlayScene::Input()
{
	if (Input::IsKeyPressed(VK_5))
	{
		UScene::ChangeScene<UEndScene>(Game::GetNextScenePtr());
	}

	m_fPlayerCharacter->Input();
}
