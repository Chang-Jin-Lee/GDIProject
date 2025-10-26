#pragma once
#include "WidgetComponent.h"

namespace Gdiplus { class SolidBrush; class Color; }
template<typename T> class TVector2; using FVector2 = TVector2<float>;

class SUIButtonComponent : public UWidgetComponent
{
public:
    SUIButtonComponent();
    ~SUIButtonComponent();

    void Initialize(Gdiplus::Color color, const FVector2& Position, const FVector2& Size, int radius);

    virtual void Release() override;

public:
    // 접근자 (pImpl로 은닉된 상태 관리)
    Gdiplus::SolidBrush* GetBrush() const;
    int GetRadius() const;
    void SetRadius(int radius);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};