#include "CameraActor.h"
#include "../Components/SceneComponent.h"
#include "../../Math/Math.h"

ACameraActor::ACameraActor()
{
}

ACameraActor::~ACameraActor()
{
}

void ACameraActor::Initialize()
{
	__super::Initialize();
}

void ACameraActor::Update()
{
	__super::Update();
}

void ACameraActor::Release()
{
	__super::Release();
}

void ACameraActor::SetCameraLocation(FVector2 location)
{
    if (auto scene = Cast<USceneComponent>(SceneComponent))
    {
        scene->SetSceneComponentLocation(location);
    }
}

FVector2 ACameraActor::GetCameraLocation()
{
    if (auto scene = Cast<USceneComponent>(SceneComponent))
    {
        return scene->GetSceneComponentLocation();
    }
    return FVector2();
}

float ACameraActor::GetCameraRotation()
{
    if (auto scene = Cast<USceneComponent>(SceneComponent))
    {
        return scene->GetSceneComponentRotation();
    }
    return 0.0f;
}

FVector2 ACameraActor::GetCameraScale()
{
    if (auto scene = Cast<USceneComponent>(SceneComponent))
    {
        return scene->GetSceneComponentScale();
    }
    return FVector2(1.0f, 1.0f);
}
