#include "Scene.h"
#include "../Actor.h"

UScene::UScene()
{
	m_objects.assign(DEFAULT_LAYER_SIZE, std::unordered_map<std::wstring, std::shared_ptr<UObject>>());
}

UScene::~UScene()
{
	m_objects.clear();
}

void UScene::Initialize()
{
	
}

void UScene::Update()
{
	for (auto objectVector : m_objects)
	{
		for (auto objectPair : objectVector)
		{
			objectPair.second->Update();
		}
	}
}

void UScene::LoadData()
{

}

void UScene::Release()
{

}

