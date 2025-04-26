#pragma once

#include <windows.h>
#include <stdio.h>
#include "../../Classes/Character.h"
#include "../../UI/UITextComponent.h"
#include "../../UI/UIButtonComponent.h"
#include "../../Experiment/StateOption.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

class ACameraActor;

namespace Renderer
{
	void Initialize(HWND hwnd);
	void Update();
	void BeginDraw();
	void Release(HWND hwnd);

	FVector2 GetResolution();
	void SetResolution(const float& width, const float& height);
	void SetMainCamera(const std::shared_ptr<ACameraActor> camera);
	std::shared_ptr<ACameraActor> GetMainCamera();

	void RenderImage(Gdiplus::Bitmap* ImageBitmap, FVector2& position, const float& rotation, const FVector2& scale, const FVector2& size);
	void RenderText(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush);
	void RenderRectRed(int x, int y, int width, int height);
	void RenderRectBlue(int x, int y, int width, int height);
	void RenderRectFill(SolidBrush& brush, FVector2& position, const FVector2& size, int radius = 5);
	void RenderRectWithRounded(SolidBrush* brush, FVector2& position, const FVector2& size, int radius = 5);

	void RenderMesh(std::shared_ptr<USceneComponent>& sceneComponent, std::shared_ptr<UStaticMeshComponent>& staticMesh);	// 액터 그리기 함수
	void RenderCharacterAnimation(ACharacter::FAnimationBundle& animationBundle, ACharacter* character); 	// 애니메이션을 위한 함수. 

	// UI 관련
	void RenderTextUI(std::shared_ptr<SUITextComponent>& textui, int parentX, int parentY);
	void RenderButtonUI(std::shared_ptr<SUIButtonComponent>& buttonui, int parentX, int parentY);

	void ClearRenderObjects();
	void SetRenderObject(std::shared_ptr<UObject> obj);

	void EndDraw();
}
