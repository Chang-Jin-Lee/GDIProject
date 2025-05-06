#pragma once

#include "CameraBase.h"

class ACameraActor : public UCamaraBase
{
public:
	ACameraActor();
	~ACameraActor();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void SetCameraLocation(FVector2 location) { Cast<USceneComponent>(SceneComponent)->SetSceneComponentLocation(location); }

	FVector2 GetCameraLocation() { return Cast<USceneComponent>(SceneComponent)->GetSceneComponentLocation(); }
	float GetCameraRotation() { return Cast<USceneComponent>(SceneComponent)->GetSceneComponentRotation(); }
	FVector2 GetCameraScale() { return Cast<USceneComponent>(SceneComponent)->GetSceneComponentScale(); }
};