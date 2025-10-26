#include "Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Experiment/AABBBox.h"
#include "../Math/Math.h"
#include "../Runtime/Renderer/Renderer.h"

AActor::AActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootStaticMeshComponent"));
    boundBox = std::make_unique<FAABBBox>();
    boundBox->SetMinVector(-1, -1);
    boundBox->SetMaxVector(-1, -1);
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
        const FVector2 loc = GetActorLocation();
        const FVector2 scale = GetActorScale();
        const FVector2 size = GetActorSize();
        boundBox->SetMinVector(loc.x, loc.y);
        boundBox->SetMaxVector(scale.x * size.x + loc.x, scale.y * size.y + loc.y);
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

Gdiplus::Bitmap* AActor::GetBitmap()
{
    if (auto StaticMeshComponentRef = Cast<UStaticMeshComponent>(StaticMeshComponent))
    {
        return StaticMeshComponentRef->GetMesh();
    }
    return nullptr;
}

FVector2 AActor::GetActorLocation() const
{
    if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
    {
        return SceneComponentRef->GetSceneComponentLocation();
    }
    return FVector2();
}

FVector2 AActor::GetActorSize()
{
    if (auto StaticMeshComponentRef = Cast<UStaticMeshComponent>(StaticMeshComponent))
    {
        return StaticMeshComponentRef->GetMeshSize();
    }
    return FVector2();
}

FVector2 AActor::GetActorScale()
{
    if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
    {
        return SceneComponentRef->GetSceneComponentScale();
    }
    return FVector2(1.0f, 1.0f);
}

float AActor::GetActorRotation()
{
    if (auto SceneComponentRef = Cast<USceneComponent>(SceneComponent))
    {
        return SceneComponentRef->GetSceneComponentRotation();
    }
    return 0.0f;
}

FAABBBox* AActor::GetBoundBox()
{
    return boundBox.get();
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
