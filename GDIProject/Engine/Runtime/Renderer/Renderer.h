#pragma once

#include <windows.h>
#include <stdio.h>
#include "../../Classes/Character.h"
#include "../../UI/UITextComponent.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

namespace Renderer
{
	void Initialize(HWND hwnd);
	void BeginDraw();
	void SetResolution(const float& width, const float& height);
	FVector2 GetResolution();
	void RenderImage(Gdiplus::Bitmap* ImageBitmap, const int& x, const int& y, const float& rotation, const float& scaleX, const float& scaleY);
	void RenderCharacterAnimation(ACharacter::FAnimationBundle& animationBundle, ACharacter* character); 	// 애니메이션을 위한 함수. 
	void RenderImageWithUI(Gdiplus::Bitmap* pImageBitmap, AActor* actor, SUITextComponent* textui);
	void RenderTextUI(SUITextComponent* textui, int x, int y);

	void RenderRectRed(int x, int y, int width, int height);
	void RenderRectBlue(int x, int y, int width, int height);
	void RenderUIButton(int x, int y, int width, int height, Gdiplus::Pen* pen);

	void RenderActor(AActor* actor);	// 액터 그리기 함수
	void RenderMesh(USceneComponent* sceneComponent, UStaticMeshComponent* staticMesh);	// 액터 그리기 함수
	void RenderActorWithUI(AActor* actor, SUITextComponent* textui);
	void EndDraw();
	void Release(HWND hwnd);
}
