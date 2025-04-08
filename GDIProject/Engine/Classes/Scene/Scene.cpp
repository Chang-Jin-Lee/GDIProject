#include "Scene.h"

UScene::UScene()
{

}

UScene::~UScene()
{

}

void UScene::Initialize()
{

}

void UScene::Update()
{

}

void UScene::LoadData()
{

}

void UScene::Release()
{

}

void UScene::ChangeScene(UScene* curScene, UScene* desScene)
{
	UScene* temp = curScene;	
	curScene = desScene;
	curScene->Initialize();
	curScene->LoadData();
	delete temp;
}
