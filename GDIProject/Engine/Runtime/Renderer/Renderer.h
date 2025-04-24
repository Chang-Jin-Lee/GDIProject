#pragma once

#include <windows.h>
#include <stdio.h>
#include "../../Classes/Character.h"
#include "../../UI/UITextComponent.h"
#include "../../Experiment/StateOption.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

class ACameraActor;

namespace Renderer
{
	void Initialize(HWND hwnd);
	void BeginDraw();
	void Release(HWND hwnd);

	FVector2 GetResolution();
	void SetResolution(const float& width, const float& height);
	void SetMainCamera(const std::shared_ptr<ACameraActor> camera);
	std::shared_ptr<ACameraActor> GetMainCamera();

	void RenderImage(Gdiplus::Bitmap* ImageBitmap, FVector2& position, const float& rotation, const FVector2& scale, const FVector2& size);
	void RenderText(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush);

	void RenderActor(AActor* actor);	// 액터 그리기 함수
	void RenderCharacterAnimation(ACharacter::FAnimationBundle& animationBundle, ACharacter* character); 	// 애니메이션을 위한 함수. 

	// UI 관련
	void RenderRectRed(int x, int y, int width, int height);
	void RenderRectBlue(int x, int y, int width, int height);
	void RenderTextUI(SUITextComponent* textui, int x, int y);
	void RenderButtonUI(int x, int y, int width, int height, Gdiplus::Pen* pen);

	void RenderMesh(USceneComponent* sceneComponent, UStaticMeshComponent* staticMesh);	// 액터 그리기 함수
	void EndDraw();
}
