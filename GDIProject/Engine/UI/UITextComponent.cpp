#include "UITextComponent.h"
#include "../Math/Math.h"
#include "../Classes/Camera/CameraActor.h"
#include "../Runtime/Renderer/Renderer.h"
#include <gdiplus.h>
#include "WidgetComponent.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

void SUITextComponent::Initialize()
{
	__super::Initialize();
}

void SUITextComponent::Update()
{
	__super::Update();
	
	//if (m_AttachedActor == nullptr) // 액터에 부착되어 있지 않을 때
	//{
	//	Renderer::RenderTextUI(this, 0, 0);
	//}
	//else // 어떤 액터에 부착되어 있을 때
	//{
	//	Renderer::RenderTextUI(this,m_AttachedActor->GetActorLocation().x,m_AttachedActor->GetActorLocation().y);
	//}
}
void SUITextComponent::Release()
{
	__super::Release();
}

SUITextComponent::SUITextComponent() : pImpl(nullptr) {}
SUITextComponent::~SUITextComponent() = default;

void SUITextComponent::Initialize(
    const std::wstring& content,
    float fontSize,
    const std::wstring& fontFamily,
    Gdiplus::Color color,
    const FVector2& Position,
    const FVector2& Size)
{
    // 기본 값들로 상세 오버로드 호출
    Initialize(content, fontSize, fontFamily, color, Position, Size,
        (Gdiplus::FontStyle)1, (Gdiplus::Unit)3, (Gdiplus::StringAlignment)1, (Gdiplus::StringAlignment)1, (Gdiplus::StringTrimming)0);
}

struct SUITextComponent::Impl {
    std::wstring content;
    float fontSize{10.0f};
    std::wstring fontFamily{L"Verdana"};
    Gdiplus::Color color{255,255,255};
    int fontStyle{1};
    int worldUnit{3};
    int fontAlignment{1};
    int fontLineAlignment{1};
    int fontTrimming{0};
    FVector2 position{0,0};
    FVector2 size{150,150};
};

void SUITextComponent::Initialize(
    const std::wstring& content,
    float fontSize,
    const std::wstring& fontFamily,
    Gdiplus::Color color,
    const FVector2& Position,
    const FVector2& Size,
    Gdiplus::FontStyle fontStyle,
    Gdiplus::Unit worldUnit,
    Gdiplus::StringAlignment fontAlignment,
    Gdiplus::StringAlignment fontLineAlignment,
    Gdiplus::StringTrimming fontTrimming)
{
    if (!pImpl) pImpl = std::make_unique<Impl>();
    pImpl->content = content;
    pImpl->fontSize = fontSize;
    pImpl->fontFamily = fontFamily;
    pImpl->color = color;
    pImpl->fontStyle = (int)fontStyle;
    pImpl->worldUnit = (int)worldUnit;
    pImpl->fontAlignment = (int)fontAlignment;
    pImpl->fontLineAlignment = (int)fontLineAlignment;
    pImpl->fontTrimming = (int)fontTrimming;
    SetPosition(Position);
    SetSize(Size);
}

// Getters/Setters
const std::wstring& SUITextComponent::GetContent() const { static std::wstring empty; return pImpl ? pImpl->content : empty; }
float SUITextComponent::GetFontSize() const { return pImpl ? pImpl->fontSize : 10.0f; }
const std::wstring& SUITextComponent::GetFontFamily() const { static std::wstring empty; return pImpl ? pImpl->fontFamily : empty; }
Gdiplus::Color SUITextComponent::GetColor() const { return pImpl ? pImpl->color : Gdiplus::Color(255,255,255); }
int SUITextComponent::GetFontStyle() const { return pImpl ? pImpl->fontStyle : 1; }
int SUITextComponent::GetWorldUnit() const { return pImpl ? pImpl->worldUnit : 3; }
int SUITextComponent::GetFontAlignment() const { return pImpl ? pImpl->fontAlignment : 1; }
int SUITextComponent::GetFontLineAlignment() const { return pImpl ? pImpl->fontLineAlignment : 1; }
int SUITextComponent::GetFontTrimming() const { return pImpl ? pImpl->fontTrimming : 0; }

void SUITextComponent::SetContent(const std::wstring& content) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->content = content; }
void SUITextComponent::SetFontSize(float size) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->fontSize = size; }
void SUITextComponent::SetFontFamily(const std::wstring& family) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->fontFamily = family; }
void SUITextComponent::SetColor(Gdiplus::Color color) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->color = color; }
void SUITextComponent::SetFontStyle(int style) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->fontStyle = style; }
void SUITextComponent::SetWorldUnit(int unit) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->worldUnit = unit; }
void SUITextComponent::SetFontAlignment(int align) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->fontAlignment = align; }
void SUITextComponent::SetFontLineAlignment(int align) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->fontLineAlignment = align; }
void SUITextComponent::SetFontTrimming(int trim) { if (!pImpl) pImpl = std::make_unique<Impl>(); pImpl->fontTrimming = trim; }
