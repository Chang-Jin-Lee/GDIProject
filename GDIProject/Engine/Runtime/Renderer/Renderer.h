#pragma once

#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

namespace Renderer
{
	int g_width = 1280;
	int g_height = 800;
	HWND g_hWnd;
	HDC g_FrontBufferDC;    // ¾Õ¸é DC
	HDC g_BackBufferDC;    // µÞ¸é DC
	HBITMAP g_BackBufferBitmap;

	ULONG_PTR g_GdiPlusToken;
	Gdiplus::Graphics* g_pBackBufferGraphics = nullptr;

	void SetResolution(const int& width, const int& height);
	void Initialize(HWND hwnd);
	void BeginDraw();
	void RenderImage(Gdiplus::Bitmap* ImageBitmap, const int& x, const int& y, const int& width, const int& height);
	void EndDraw();
	void Release(HWND hwnd);
}
