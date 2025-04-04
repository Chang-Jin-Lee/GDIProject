#pragma once
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

class AActor
{
public:
	AActor();
	~AActor();

	void LoadData(std::wstring baseDir, std::wstring fileName);

private:
	USceneComponent* SceneComponent;
	UStaticMeshComponent* StaticMeshComponent;
};