#pragma once

#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include "../../Classes/Actor.h"
#include "../../UI/UIText.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

namespace Renderer
{
	void SetResolution(const int& width, const int& height);
	void Initialize(HWND hwnd);
	void BeginDraw();
	void RenderImage(Gdiplus::Bitmap* ImageBitmap, const int& x, const int& y, const float& rotation, const float& scaleX, const float& scaleY);
	void RenderImage(Gdiplus::Bitmap* pImageBitmap, AActor* actor); 	// 애니메이션을 위한 함수. 
	void RenderImageWithUI(Gdiplus::Bitmap* pImageBitmap, AActor* actor, SUIText* textui);
	void RenderTextUI(SUIText* textui, int x, int y);

	void RenderRectRed(int x, int y, int width, int height);
	void RenderRectBlue(int x, int y, int width, int height);

	void RenderActor(AActor* actor);	// 액터 그리기 함수
	void EndDraw();
	void Release(HWND hwnd);
	
	int GetWidth();
	void SetWidth(int value);
	int GetHeight();
	void SetHeight(int value);
}
