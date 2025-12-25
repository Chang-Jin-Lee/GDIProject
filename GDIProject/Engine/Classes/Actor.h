#pragma once
#include <vector>
#include <memory>
#include "Object.h"

template<typename T> class TVector2; using FVector2 = TVector2<float>;

// Forward declarations to reduce header dependencies
namespace Gdiplus { class Bitmap; }
class USceneComponent;
class UStaticMeshComponent;
class UWidget;
class FAABBBox;

class AActor : public UObject
{
public:
    AActor();
    ~AActor();
    void LoadStaticMeshData(std::wstring baseDir, std::wstring fileName);

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Release() override;
    virtual void LoadData();

    // Accessors moved to cpp to avoid including heavy headers here
    Gdiplus::Bitmap* GetBitmap();
    FVector2 GetActorLocation() const;
    FVector2 GetActorSize();
    FVector2 GetActorScale();
    float GetActorRotation();

    FAABBBox* GetBoundBox();
    
    void SetActorScale(float x, float y);
    void SetActorSize(float x, float y);
    void SetActorLocation(float x, float y);
    void SetActorLocation(const FVector2& Position);
    void SetActorRotation(float value);
    
public:
    std::weak_ptr<USceneComponent> SceneComponent;
    std::weak_ptr<UStaticMeshComponent> StaticMeshComponent;
    std::unique_ptr<FAABBBox> boundBox;
    std::vector<std::weak_ptr<UWidget>> attachedWidgets;
    bool bSelected = false;
    bool bVisible = true;
};