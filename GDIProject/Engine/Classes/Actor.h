#pragma once
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Object.h"
#include "../Math/Math.h"
#include "../Experiment/AABBBox.h"

class AActor : public UObject
{
public:
	AActor();
	~AActor();
	void LoadStaticMeshData(std::wstring baseDir, std::wstring fileName);

	virtual void Initialize() override;
	virtual void Update();
	virtual void Release() override;

	virtual void LoadData();

	Gdiplus::Bitmap* GetBitmap() { return StaticMeshComponent->mesh; }

	FVector2 GetActorLocation() const { return SceneComponent->GetSceneComponentLocation(); }
	FVector2 GetActorSize() { return StaticMeshComponent->GetMeshSize();}
	FVector2 GetActorScale() { return SceneComponent->GetSceneComponentScale(); }
	float GetActorRotation() { return SceneComponent->GetSceneComponentRotation(); }

	FAABBBox* GetBoundBox() { return &boundBox; }
	
	void SetActorScale(float x, float y);
	void SetActorSize(float x, float y);
	void SetActorLocation(float x, float y);
	void SetActorRotation(float value);
	
private:
	USceneComponent* SceneComponent;
	UStaticMeshComponent* StaticMeshComponent;
	FAABBBox boundBox;
};