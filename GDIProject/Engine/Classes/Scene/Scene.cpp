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
	for (auto objectPair : m_objects)
	{
		objectPair.second->Update();
	}
}

void UScene::LoadData()
{

}

void UScene::Release()
{

}

