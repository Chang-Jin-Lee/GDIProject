// Renderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Renderer.h"

namespace Renderer
{
	int g_width = 1280;
	int g_height = 800;
	HWND g_hWnd;
	HDC g_FrontBufferDC;    // 앞면 DC
	HDC g_BackBufferDC;    // 뒷면 DC
	HBITMAP g_BackBufferBitmap;

	ULONG_PTR g_GdiPlusToken;
	Gdiplus::Graphics* g_pBackBufferGraphics = nullptr;

	void SetResolution(const int& width, const int& height)
	{
		g_width = width;
		g_height = height;
	}

	void Initialize(HWND hwnd)
	{
		////////Renderer::Initialize
		g_FrontBufferDC = GetDC(hwnd); //윈도우 클라이언트 영역의 DeviceContext얻기
		g_BackBufferDC = CreateCompatibleDC(g_FrontBufferDC); // 호환되는 DeviceContext 생성
		g_BackBufferBitmap = CreateCompatibleBitmap(g_FrontBufferDC, g_width, g_height); // 메모리 영역생성
		SelectObject(g_BackBufferDC, g_BackBufferBitmap); // MemDC의 메모리영역 지정

		Gdiplus::GdiplusStartupInput gsi;
		Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
		g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);
	}

	void BeginDraw()
	{
		PatBlt(g_BackBufferDC, 0, 0, g_width, g_height, BLACKNESS);
	}

	void RenderImage(Gdiplus::Bitmap* ImageBitmap, const int& x, const int& y, const int& width, const int& height)
	{
		if(ImageBitmap)
			g_pBackBufferGraphics->DrawImage(ImageBitmap, x, y, width, height);
	}

	void EndDraw()
	{
		BitBlt(g_FrontBufferDC, 0, 0, g_width, g_height, g_BackBufferDC, 0, 0, SRCCOPY);
	}

	void Release(HWND hwnd)
	{
		DeleteObject(g_BackBufferBitmap);
		DeleteDC(g_BackBufferDC);
		ReleaseDC(hwnd, g_FrontBufferDC);

		if (g_pBackBufferGraphics != nullptr)
			delete g_pBackBufferGraphics;
		Gdiplus::GdiplusShutdown(g_GdiPlusToken);
	}
	int GetWidth()
	{
		return g_width;
	}
	void SetWidth(int value)
	{
		g_width = value;
	}
	int GetHeight()
	{
		return g_height;
	}
	void SetHeight(int value)
	{
		g_height = value;
	}
}
