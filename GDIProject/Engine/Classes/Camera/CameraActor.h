#pragma once

#include "CameraBase.h"

template<typename T> class TVector2; using FVector2 = TVector2<float>;

class ACameraActor : public UCamaraBase
{
public:
    ACameraActor();
    ~ACameraActor();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Release() override;

    void SetCameraLocation(FVector2 location);
    FVector2 GetCameraLocation();
    float GetCameraRotation();
    FVector2 GetCameraScale();
};