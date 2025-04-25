#include "Scene.h"
#include "../../Runtime/Renderer/Renderer.h"

UScene::UScene()
{
	m_objects.assign(DEFAULT_LAYER_SIZE, std::unordered_map<std::wstring, std::shared_ptr<AActor>>());
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
	Renderer::ClearRenderObjects();
	for (auto objectMap : m_objects)
	{
		for (auto objectPair : objectMap)
		{
			objectPair.second->Update();
			// 여기에 컬링 추가
			Renderer::SetRenderObject(objectPair.second);
		}
	}
}

void UScene::LoadData()
{

}

void UScene::Release()
{

}

