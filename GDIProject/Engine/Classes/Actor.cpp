#include "Actor.h"
#include "../Runtime/Renderer/Renderer.h"

AActor::AActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootStaticMeshComponent"));
	boundBox.SetMinVector(-1, -1);
	boundBox.SetMaxVector(-1, -1);
	SetName((wchar_t*)L"AActor");
}

AActor::~AActor()
{
	SceneComponent.reset();
	StaticMeshComponent.reset();
}

void AActor::LoadStaticMeshData(std::wstring baseDir, std::wstring fileName)
{
	if (auto StaticMeshComponentRef = Cast<UStaticMeshComponent>(StaticMeshComponent))
	{
		StaticMeshComponentRef->LoadData(baseDir, fileName);
	}
}

void AActor::Initialize()
{
	__super::Initialize();
}

void AActor::Update()
{
	__super::Update();
	
	//Renderer::RenderMesh(SceneComponent, StaticMeshComponent);
	if (bStatic == false)
	{
		boundBox.m_minVector.x = GetActorLocation().x;
		boundBox.m_minVector.y = GetActorLocation().y;
		boundBox.m_maxVector.x = GetActorScale().x * GetActorSize().x + GetActorLocation().x;
		boundBox.m_maxVector.y = GetActorScale().y * GetActorSize().y + GetActorLocation().y;
	}
}

void AActor::Release()
{
	__super::Release();
	SceneComponent.reset();
	StaticMeshComponent.reset();
}

void AActor::LoadData()
{
}

void AActor::SetActorScale(float x, float y)
{
	if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
	{
		SceneComponentRef->SetSceneComponentScale(x, y);
	}
}

void AActor::SetActorSize(float x, float y)
{
	if (auto StaticMeshComponentRef = Cast<UStaticMeshComponent>(StaticMeshComponent))
	{
		StaticMeshComponentRef->SetMeshSize(x, y);
	}
}

void AActor::SetActorLocation(float x, float y)
{
	if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
	{
		SceneComponentRef->SetSceneComponentLocation(x, y);
	}
}

void AActor::SetActorLocation(const FVector2& Position)
{
	if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
	{
		SceneComponentRef->SetSceneComponentLocation(Position.x, Position.y);
	}
}

void AActor::SetActorRotation(float value)
{
	if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
	{
		SceneComponentRef->SetSceneComponentRotation(value);
	}
}
