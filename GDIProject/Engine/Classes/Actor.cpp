#include "Actor.h"

AActor::AActor()
{
	SceneComponent = new USceneComponent();
	StaticMeshComponent = new UStaticMeshComponent();
	boundBox.SetMinVector(0, 0);
	boundBox.SetMaxVector(0, 0);
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
	__super::Initialize();
}

void AActor::Update()
{
	__super::Update();
	//g_pBackBufferGraphics->DrawImage(g_pImageBitmap[g_eCurrentCharacter][m_iplayidx], (int)(g_width / 2 - witdh[m_iplayidx] / 2), (int)(g_height / 2 - height[m_iplayidx] / 2), witdh[m_iplayidx], height[m_iplayidx]);
	boundBox.m_minVector.x = GetActorLocation().x;
	boundBox.m_minVector.y = GetActorLocation().y;
	boundBox.m_maxVector.x = GetActorScale().x * GetActorSize().x + GetActorLocation().x;
	boundBox.m_maxVector.y = GetActorScale().y * GetActorSize().y + GetActorLocation().y;
}

void AActor::Release()
{
	__super::Release();
}

void AActor::LoadData()
{
}

void AActor::SetActorScale(float x, float y)
{
	SceneComponent->SetSceneComponentScale(x, y);
}

void AActor::SetActorSize(float x, float y)
{
	StaticMeshComponent->SetMeshSize(x, y);
}

void AActor::SetActorLocation(float x, float y)
{
	SceneComponent->SetSceneComponentLocation(x, y);
}

void AActor::SetActorRotation(float value)
{
	SceneComponent->SetSceneComponentRotation(value);
}
