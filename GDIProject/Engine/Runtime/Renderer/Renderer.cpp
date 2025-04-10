// Renderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Renderer.h"
#include "../../Classes/Actor.h"
#include <iostream>

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

	void RenderImage(Gdiplus::Bitmap* pImageBitmap, const int& x, const int& y, const float& rotation, const float& scaleX, const float& scaleY)
	{
		if (pImageBitmap && g_pBackBufferGraphics)
		{

			int imgWidth = pImageBitmap->GetWidth();
			int imgHeight = pImageBitmap->GetHeight();

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth / 2), float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)x, (float)y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix);

			g_pBackBufferGraphics->DrawImage(pImageBitmap, x, y, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	// 애니메이션을 할때는 비트맵을 한장씩 넘겨야 하니까 Bitmap 받기
	void RenderImage(Gdiplus::Bitmap* pImageBitmap, AActor* actor)
	{
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const int x = actor->GetActorLocation().x;
			const int y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = pImageBitmap->GetWidth();
			int imgHeight = pImageBitmap->GetHeight();

			std::cout << x << " " << y << '\n';

			Gdiplus::PointF center = Gdiplus::PointF(x + float(imgWidth / 2), y + float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)x, (float)y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix);	// 매트릭스 적용.

			wchar_t* name = actor->GetName();
			if (name != nullptr)
			{
				Gdiplus::FontFamily fontFamily(L"Verdana");
				Gdiplus::Font font(&fontFamily, 12, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
				Gdiplus::StringFormat stringFormat;
				stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);	// 영역의 상단에 맞춰지게 
				stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear); // 다음 줄로 갱신될 때 왼쪽부터 써지기
				stringFormat.SetTrimming(Gdiplus::StringTrimmingNone); // trim 안함
				Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));
				g_pBackBufferGraphics->DrawString(name, wcslen(name), &font, Gdiplus::RectF(x - 60, y - 25, 150, 50), &stringFormat, &brush);
			}

			g_pBackBufferGraphics->DrawImage(pImageBitmap, x, y, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	// 액터 그리기
	void RenderActor(AActor* actor)
	{
		Gdiplus::Bitmap* pImageBitmap = actor->GetBitmap();
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const int x = actor->GetActorLocation().x;
			const int y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = actor->GetActorSize().x;
			int imgHeight = actor->GetActorSize().y;

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth / 2), float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)x, (float)y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix); // 매트릭스 적용.

			wchar_t* name = actor->GetName();
			if (name != nullptr)
			{
				Gdiplus::FontFamily fontFamily(L"Verdana");
				Gdiplus::Font font(&fontFamily, 15, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
				Gdiplus::StringFormat stringFormat;
				stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);	// 영역의 상단에 맞춰지게 
				stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear); // 다음 줄로 갱신될 때 왼쪽부터 써지기
				stringFormat.SetTrimming(Gdiplus::StringTrimmingNone); // trim 안함
				Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255));
				g_pBackBufferGraphics->DrawString(name, wcslen(name), &font, Gdiplus::RectF(x, y, 150, 150), &stringFormat, &brush);
			}

			g_pBackBufferGraphics->DrawImage(pImageBitmap, x, y, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	void RenderActorAnimation(AActor* actor)
	{
		Gdiplus::Bitmap* pImageBitmap = actor->GetBitmap();
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const int x = actor->GetActorLocation().x;
			const int y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = pImageBitmap->GetWidth() / 2;
			int imgHeight = pImageBitmap->GetHeight() / 2;

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth), float(imgHeight));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)x, (float)y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix);

			g_pBackBufferGraphics->DrawImage(pImageBitmap, x, y, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
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
