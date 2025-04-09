#include "Actor.h"

AActor::AActor()
{
	SceneComponent = new USceneComponent();
	StaticMeshComponent = new UStaticMeshComponent();
}

AActor::~AActor()
{
	delete SceneComponent;
	delete StaticMeshComponent;
}

void AActor::LoadStaticMeshData(std::wstring baseDir, std::wstring fileName)
{
	StaticMeshComponent->LoadData(baseDir,fileName);
}

void AActor::Initialize()
{
}

void AActor::Update()
{
	//g_pBackBufferGraphics->DrawImage(g_pImageBitmap[g_eCurrentCharacter][m_iplayidx], (int)(g_width / 2 - witdh[m_iplayidx] / 2), (int)(g_height / 2 - height[m_iplayidx] / 2), witdh[m_iplayidx], height[m_iplayidx]);
}

void AActor::Release()
{
}

void AActor::LoadData()
{
}
