#pragma once
#include "WidgetComponentBase.h"

class AActor;

template<typename T> class TVector2; using FVector2 = TVector2<float>;

class UWidgetComponent : public UWidgetComponentBase
{
public:
    ~UWidgetComponent();
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void AttachedUIToActor(const std::weak_ptr<UObject>& object);

    // Visibility
    bool IsVisible() const;
    void SetVisible(bool visible);

    // Position/Size API
    void SetPosition(const FVector2& position);
    void SetSize(const FVector2& size);
    FVector2 GetPosition() const;
    FVector2 GetSize() const;

    enum class WidgetRenderType
    {
        Camera, // 화면 고정(UI 공간)
        World   // 월드 좌표(줌/카메라 영향)
    };
    void SetWidgetRenderType(WidgetRenderType type);
    WidgetRenderType GetWidgetRenderType() const;

public:
    std::weak_ptr<AActor> m_AttachedActor;

private:
    struct Impl;
    struct ImplDeleter { void operator()(Impl*) noexcept; };
    std::unique_ptr<Impl, ImplDeleter> pImpl;
};