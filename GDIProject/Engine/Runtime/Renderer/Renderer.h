#pragma once

#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

namespace Renderer
{
	void SetResolution(const int& width, const int& height);
	void Initialize(HWND hwnd);
	void BeginDraw();
	void RenderImage(Gdiplus::Bitmap* ImageBitmap, const int& x, const int& y, const int& width, const int& height);
	void EndDraw();
	void Release(HWND hwnd);
	
	int GetWidth();
	void SetWidth(int value);
	int GetHeight();
	void SetHeight(int value);
}
