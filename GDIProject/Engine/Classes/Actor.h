#pragma once
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Object.h"
#include "../Math/Math.h"

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
	void SetActorLocation(float x, float y) { SceneComponent->SetSceneComponentLocation(x, y); }

	FVector2 GetActorScale() { return StaticMeshComponent->GetMeshScale();}
	void SetActorScale(float x, float y) { StaticMeshComponent->SetMeshScale(x, y); }
	
private:
	USceneComponent* SceneComponent;
	UStaticMeshComponent* StaticMeshComponent;
};