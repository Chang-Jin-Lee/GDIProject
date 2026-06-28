#include "UIButtonComponent.h"
#include "../Math/Math.h"
#include <gdiplus.h>
#include "WidgetComponent.h"
#include "../Runtime/Renderer/Renderer.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

struct SUIButtonComponent::Impl {
    Gdiplus::SolidBrush* brush{nullptr};
    int radius{5};
    float pressT{0.0f};
};

SUIButtonComponent::SUIButtonComponent() : pImpl(std::make_unique<Impl>()) {
    pImpl->brush = nullptr;
}

SUIButtonComponent::~SUIButtonComponent() {
    if (pImpl && pImpl->brush) {
        if (Renderer::IsGdiValid()) {
            delete pImpl->brush;
        }
        pImpl->brush = nullptr;
    }
}

void SUIButtonComponent::Initialize(Gdiplus::Color color, const FVector2& Position, const FVector2& Size, int radius) {
    __super::Initialize();
    if (!pImpl) pImpl = std::make_unique<Impl>();
    if (pImpl->brush == nullptr && Renderer::IsGdiValid()) pImpl->brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255));
    pImpl->brush->SetColor(color);
    SetPosition(Position);
    SetSize(Size);
    pImpl->radius = radius;
}

void SUIButtonComponent::Release() {
    __super::Release();
    if (pImpl && pImpl->brush) {
        if (Renderer::IsGdiValid()) {
            delete pImpl->brush;
        }
        pImpl->brush = nullptr;
    }
}

Gdiplus::SolidBrush* SUIButtonComponent::GetBrush() const { return pImpl ? pImpl->brush : nullptr; }
int SUIButtonComponent::GetRadius() const { return pImpl ? pImpl->radius : 5; }
void SUIButtonComponent::SetRadius(int radius) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->radius = radius; }

void SUIButtonComponent::TriggerPress() { if (pImpl) pImpl->pressT = 1.0f; }
float SUIButtonComponent::GetPressT() const { return pImpl ? pImpl->pressT : 0.0f; }
void SUIButtonComponent::UpdateAnim(float dt)
{
    if (!pImpl || pImpl->pressT <= 0.0f) return;
    pImpl->pressT -= dt * 8.0f;
    if (pImpl->pressT < 0.0f) pImpl->pressT = 0.0f;
}


