#pragma once

#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include "../../Classes/Actor.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

namespace Renderer
{
	void SetResolution(const int& width, const int& height);
	void Initialize(HWND hwnd);
	void BeginDraw();
	void RenderImage(Gdiplus::Bitmap* ImageBitmap, const int& x, const int& y, const float& rotation, const float& scaleX, const float& scaleY);
	void RenderImage(Gdiplus::Bitmap* pImageBitmap, AActor* actor); 	// 애니메이션을 위한 함수. 
	void RenderActor(AActor* actor);	// 액터 그리기 함수
	void EndDraw();
	void Release(HWND hwnd);
	
	int GetWidth();
	void SetWidth(int value);
	int GetHeight();
	void SetHeight(int value);
}
