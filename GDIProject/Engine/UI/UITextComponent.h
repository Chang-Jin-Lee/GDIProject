#pragma once
#include "WidgetComponent.h"


namespace Gdiplus { class Color; enum FontStyle; enum Unit; enum StringAlignment; enum StringTrimming; }
template<typename T> class TVector2; using FVector2 = TVector2<float>;

class SUITextComponent : public UWidgetComponent
{
public:
    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Release() override;

    SUITextComponent();
    ~SUITextComponent();

    // 간편 오버로드: 기존 호출부 호환
    void Initialize(
        const std::wstring& content,
        float fontSize,
        const std::wstring& fontFamily,
        Gdiplus::Color color,
        const FVector2& Position,
        const FVector2& Size
    );

    // 상세 설정 오버로드
    void Initialize(
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
        Gdiplus::StringTrimming fontTrimming
    );

public:
    // 접근자 (pImpl 은닉)
    const std::wstring& GetContent() const;
    float GetFontSize() const;
    const std::wstring& GetFontFamily() const;
    Gdiplus::Color GetColor() const;
    int GetFontStyle() const;
    int GetWorldUnit() const;
    int GetFontAlignment() const;
    int GetFontLineAlignment() const;
    int GetFontTrimming() const;

    void SetContent(const std::wstring& content);
    void SetFontSize(float size);
    void SetFontFamily(const std::wstring& family);
    void SetColor(Gdiplus::Color color);
    void SetFontStyle(int style);
    void SetWorldUnit(int unit);
    void SetFontAlignment(int align);
    void SetFontLineAlignment(int align);
    void SetFontTrimming(int trim);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};