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

	void SetCameraLocation(FVector2 location) { SceneComponent->SetSceneComponentLocation(location); }

	FVector2 GetCameraLocation() { return SceneComponent->GetSceneComponentLocation(); }
	float GetCameraRotation() { return SceneComponent->GetSceneComponentRotation(); }
	FVector2 GetCameraScale() { return SceneComponent->GetSceneComponentScale(); }
};