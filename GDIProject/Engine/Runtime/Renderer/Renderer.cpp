// Renderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Renderer.h"
#include "../../Classes/Actor.h"
#include "../../Math/Math.h"
#include <iostream>


namespace Renderer
{
	FVector2 g_resolution = FVector2(1280,800);
	HWND g_hWnd;
	HDC g_FrontBufferDC;    // 앞면 DC
	HDC g_BackBufferDC;    // 뒷면 DC
	HBITMAP g_BackBufferBitmap;

	ULONG_PTR g_GdiPlusToken;
	Gdiplus::Graphics* g_pBackBufferGraphics = nullptr;

	const Gdiplus::Pen* m_redPen = nullptr; // 빨간펜
	const Gdiplus::Pen* m_BluePen = nullptr; // 파란펜

	void SetResolution(const float& width, const float& height)
	{
		g_resolution.x = width;
		g_resolution.y = height;
	}

	FVector2 GetResolution()
	{
		return g_resolution;
	}

	void Initialize(HWND hwnd)
	{
		////////Renderer::Initialize
		g_FrontBufferDC = GetDC(hwnd); //윈도우 클라이언트 영역의 DeviceContext얻기
		g_BackBufferDC = CreateCompatibleDC(g_FrontBufferDC); // 호환되는 DeviceContext 생성
		g_BackBufferBitmap = CreateCompatibleBitmap(g_FrontBufferDC, (int)g_resolution.x, (int)g_resolution.y); // 메모리 영역생성
		SelectObject(g_BackBufferDC, g_BackBufferBitmap); // MemDC의 메모리영역 지정

		Gdiplus::GdiplusStartupInput gsi;
		Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
		g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);

		if(m_redPen == nullptr)
			m_redPen = new Gdiplus::Pen(Color(255, 0, 0), 1.0f); // 빨간펜
		if(m_BluePen == nullptr)
			m_BluePen = new Gdiplus::Pen(Color(0, 0, 255), 1.0f); // 파란펜
	}

	void BeginDraw()
	{
		PatBlt(g_BackBufferDC, 0, 0, (int)g_resolution.x, (int)g_resolution.y, BLACKNESS);
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

			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	void RenderRectRed(int x, int y, int width, int height)
	{
		const Gdiplus::Rect rect(x, y, width, height);
		g_pBackBufferGraphics->DrawRectangle(m_redPen, rect);
	}

	void RenderRectBlue(int x, int y, int width, int height)
	{
		const Gdiplus::Rect rect(x, y, width, height);
		g_pBackBufferGraphics->DrawRectangle(m_BluePen, rect);
	}

	void RenderUIButton(int x, int y, int width, int height, Gdiplus::Pen* pen)
	{
		const Gdiplus::Rect rect(x, y, width, height);
		g_pBackBufferGraphics->DrawRectangle(pen, rect);
	}

	// 애니메이션을 할때는 비트맵을 한장씩 넘겨야 하니까 Bitmap 받기
	void RenderImage(Gdiplus::Bitmap* pImageBitmap, AActor* actor)
	{
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const float x = actor->GetActorLocation().x;
			const float y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = pImageBitmap->GetWidth();
			int imgHeight = pImageBitmap->GetHeight();

			Gdiplus::PointF center = Gdiplus::PointF(x + float(imgWidth / 2), y + float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate(x, y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix);	// 매트릭스 적용.

			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	void RenderTextUI(SUIText* textui, int _x, int _y)
	{
		wchar_t* name = textui->m_content;
		if (name != nullptr)
		{
			float x = _x + textui->m_Position.x;
			float y = _y + textui->m_Position.y;
			float width = textui->m_Size.x;
			float height = textui->m_Size.y;
			Gdiplus::FontFamily fontFamily(textui->m_fontFamily);
			Gdiplus::Font font(&fontFamily, textui->m_fontSize, textui->m_efontStyle, textui->m_eworldUnit);
			Gdiplus::StringFormat stringFormat;
			stringFormat.SetAlignment(textui->m_fontAlignment);	// 영역의 상단에 맞춰지게 
			stringFormat.SetLineAlignment(textui->m_fontLineAlignment); // 다음 줄로 갱신될 때 왼쪽부터 써지기
			stringFormat.SetTrimming(textui->m_fontTrimming); // trim 안함
			Gdiplus::SolidBrush brush(textui->m_color);
			RenderRectRed((int)x, (int)y, (int)width, (int)height);
			g_pBackBufferGraphics->DrawString(name, (INT)wcslen(name), &font, Gdiplus::RectF(x, y, width, height), &stringFormat, &brush);
		}
	}

	void RenderImageWithUI(Gdiplus::Bitmap* pImageBitmap, AActor* actor, SUIText* textui)
	{
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const float x = actor->GetActorLocation().x;
			const float y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = (int)actor->GetActorSize().x;
			int imgHeight = (int)actor->GetActorSize().y;

			//int imgWidth = pImageBitmap->GetWidth();
			//int imgHeight = pImageBitmap->GetHeight();

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth / 2), float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)x, (float)y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix);	// 매트릭스 적용.
		
			RenderRectRed(0, 0, imgWidth, imgHeight);
			RenderTextUI(textui, 0, 0);

			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	// 액터 그리기
	void RenderActor(AActor* actor)
	{
		Gdiplus::Bitmap* pImageBitmap = actor->GetBitmap();
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const float x = actor->GetActorLocation().x;
			const float y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = (int)actor->GetActorSize().x;
			int imgHeight = (int)actor->GetActorSize().y;

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth / 2), float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate(x, y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix); // 매트릭스 적용.
			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	void RenderActorWithUI(AActor* actor, SUIText* textui)
	{
		Gdiplus::Bitmap* pImageBitmap = actor->GetBitmap();
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const float x = actor->GetActorLocation().x;
			const float y = actor->GetActorLocation().y;
			const float rotation = actor->GetActorRotation();
			const float scaleX = actor->GetActorScale().x;
			const float scaleY = actor->GetActorScale().y;

			int imgWidth = (int)actor->GetActorSize().x;
			int imgHeight = (int)actor->GetActorSize().y;

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth / 2), float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)x, (float)y);
			matrix.Scale(scaleX, scaleY);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix); // 매트릭스 적용.
			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);

			RenderRectRed(0, 0, imgWidth, imgHeight);
			RenderTextUI(textui, 0, 0);

			g_pBackBufferGraphics->ResetTransform();
		}
	}
	void RenderActorAnimation(AActor* actor)
	{
		Gdiplus::Bitmap* pImageBitmap = actor->GetBitmap();
		if (pImageBitmap && g_pBackBufferGraphics)
		{
			const float x = actor->GetActorLocation().x;
			const float y = actor->GetActorLocation().y;
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
			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	void EndDraw()
	{
		BitBlt(g_FrontBufferDC, 0, 0, (int)g_resolution.x, (int)g_resolution.y, g_BackBufferDC, 0, 0, SRCCOPY);
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
}
