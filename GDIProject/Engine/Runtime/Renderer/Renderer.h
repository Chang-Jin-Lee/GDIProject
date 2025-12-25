#pragma once

#include <windows.h>
#include <stdio.h>
#include <memory>

template<typename T> class TVector2; using FVector2 = TVector2<float>;
class ACharacter;               // from Classes/Character.h
class SUITextComponent;         // from UI/UITextComponent.h
class SUIButtonComponent;       // from UI/UIButtonComponent.h

// GDI+ ���� ���� (�������? ���� �������� ����)
namespace Gdiplus {
    class Bitmap; class Font; class StringFormat; class SolidBrush;
}

class ACameraActor;
class USceneComponent;
class UStaticMeshComponent;
class UObject;

namespace Renderer
{
	void Initialize(HWND hwnd);
	void Update();
	void BeginDraw();
	void Release(HWND hwnd);

    FVector2 GetResolution();
	void SetResolution(const float& width, const float& height);
	void SetMainCamera(const std::weak_ptr<ACameraActor> camera);
	bool IsGdiValid();
	std::weak_ptr<ACameraActor> GetMainCamera();

    void RenderImage(Gdiplus::Bitmap* ImageBitmap, FVector2& position, const float& rotation, const FVector2& scale, const FVector2& size, const bool& bSelected);
    void RenderText(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush);
	// ȭ�� ���� �ؽ�Ʈ(ī�޶� ������/�� ������)
	void RenderTextScreen(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush);
	void RenderRectRed(int x, int y, int width, int height);
	void RenderRectBlue(int x, int y, int width, int height);
    void RenderRectFill(Gdiplus::SolidBrush& brush, FVector2& position, const FVector2& size, int radius = 5);
    void RenderRectWithRounded(Gdiplus::SolidBrush* brush, FVector2& position, const FVector2& size, int radius = 5);
    // UI�� �� ���� ������. ī�޶� �����Ǿ� ����
    void RenderRectWithRoundedUI(Gdiplus::SolidBrush* brush, FVector2& position, const FVector2& size, int radius = 5);

	void RenderMesh(const std::shared_ptr<USceneComponent>& sceneComponent, const std::shared_ptr<UStaticMeshComponent>& staticMesh, const bool& bSelected);	// ���� �׸��� �Լ�
    void RenderCharacterAnimation(ACharacter* character); 	// �ִϸ��̼��� ���� �Լ�. 

	// UI ����
	void RenderTextUI(const std::shared_ptr<SUITextComponent>& textui, int parentX, int parentY);
    void RenderButtonUI(const std::shared_ptr<SUIButtonComponent>& buttonui, int parentX, int parentY);

	void ClearRenderObjects();
	void SetRenderObject(std::weak_ptr<UObject> obj);

	void EndDraw();
}
