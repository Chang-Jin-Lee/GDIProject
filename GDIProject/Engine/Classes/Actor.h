#pragma once
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Math/Math.h"
#include "../Experiment/AABBBox.h"
#include "Object.h"

class UWidget;

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

	//std::vector<std::function<UObject>()> functions;

	Gdiplus::Bitmap* GetBitmap() { return StaticMeshComponent->mesh; }

	FVector2 GetActorLocation() const { return SceneComponent->GetSceneComponentLocation(); }
	FVector2 GetActorSize() { return StaticMeshComponent->GetMeshSize();}
	FVector2 GetActorScale() { return SceneComponent->GetSceneComponentScale(); }
	float GetActorRotation() { return SceneComponent->GetSceneComponentRotation(); }

	FAABBBox* GetBoundBox() { return &boundBox; }
	
	void SetActorScale(float x, float y);
	void SetActorSize(float x, float y);
	void SetActorLocation(float x, float y);
	void SetActorLocation(const FVector2& Position);
	void SetActorRotation(float value);
	
public:
	std::shared_ptr<USceneComponent> SceneComponent;
	std::shared_ptr<UStaticMeshComponent> StaticMeshComponent;
	FAABBBox boundBox;
	std::vector<std::shared_ptr<UWidget>> attachedWidgets;
};