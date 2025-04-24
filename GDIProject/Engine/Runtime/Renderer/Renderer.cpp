// Renderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Renderer.h"
#include "../../Classes/Actor.h"
#include "../../Math/Math.h"
#include "../../Classes/Camera/CameraActor.h"
#include <iostream>


namespace Renderer
{
	FVector2 g_resolution = FVector2(1280, 800);
	HWND g_hWnd;
	HDC g_FrontBufferDC;    // 앞면 DC
	HDC g_BackBufferDC;    // 뒷면 DC

	HBITMAP g_BackBufferBitmap;
	ULONG_PTR g_GdiPlusToken;
	Gdiplus::Graphics* g_pBackBufferGraphics = nullptr;

	std::shared_ptr<ACameraActor> g_mainCamera;

	const Gdiplus::Pen* m_redPen = nullptr; // 빨간펜
	const Gdiplus::Pen* m_BluePen = nullptr; // 파란펜

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

		if (m_redPen == nullptr)
			m_redPen = new Gdiplus::Pen(Color(255, 0, 0), 1.0f); // 빨간펜
		if (m_BluePen == nullptr)
			m_BluePen = new Gdiplus::Pen(Color(0, 0, 255), 1.0f); // 파란펜
	}

	void BeginDraw()
	{
		PatBlt(g_BackBufferDC, 0, 0, (int)g_resolution.x, (int)g_resolution.y, BLACKNESS);
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

	void RenderImage(Gdiplus::Bitmap* pImageBitmap, FVector2& position, const float& rotation, const FVector2& scale, const FVector2& size)
	{
		if (position.x < g_mainCamera->GetCameraLocation().x * 1.3f ||
			position.x > g_mainCamera->GetCameraLocation().x * 1.3f + g_resolution.x ||
			position.y < g_mainCamera->GetCameraLocation().y * 1.3f ||
			position.y > g_mainCamera->GetCameraLocation().y * 1.3f + g_resolution.y
			)
			return;

		const int imgWidth = size.x;
		const int imgHeight = size.y;

		FVector2 finalPos = position;
		if (g_mainCamera)
			finalPos = position - g_mainCamera->GetCameraLocation();

		if (pImageBitmap && g_pBackBufferGraphics)
		{

			Gdiplus::PointF center = Gdiplus::PointF(float(imgWidth / 2), float(imgHeight / 2));
			Gdiplus::Matrix matrix;
			matrix.Translate((float)finalPos.x, (float)finalPos.y);
			matrix.Scale(scale.x, scale.y);
			matrix.RotateAt(rotation, center);
			g_pBackBufferGraphics->SetTransform(&matrix);
			g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, imgWidth, imgHeight);
			g_pBackBufferGraphics->ResetTransform();
		}
	}

	void RenderText(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush)
	{
		if (position.x < g_mainCamera->GetCameraLocation().x * 1.3f ||
			position.x > g_mainCamera->GetCameraLocation().x * 1.3f + g_resolution.x ||
			position.y < g_mainCamera->GetCameraLocation().y * 1.3f ||
			position.y > g_mainCamera->GetCameraLocation().y * 1.3f + g_resolution.y
			)
			return;

		FVector2 finalPos = position;
		if (g_mainCamera)
			finalPos = position - g_mainCamera->GetCameraLocation();

		Gdiplus::PointF center = Gdiplus::PointF(float(size.x / 2), float(size.y / 2));
		Gdiplus::Matrix matrix;
		matrix.Translate(finalPos.x, finalPos.y);
		matrix.Scale(1.0f, 1.0f);
		matrix.RotateAt(0, center);
		g_pBackBufferGraphics->SetTransform(&matrix); // 매트릭스 적용.
		RenderRectRed(0, 0, (int)size.x, (int)size.y);
		g_pBackBufferGraphics->DrawString(content, (INT)wcslen(content), &font, Gdiplus::RectF(0, 0, size.x, size.y), &stringFormat, &brush);
		g_pBackBufferGraphics->ResetTransform();
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

	void RenderButtonUI(int x, int y, int width, int height, Gdiplus::Pen* pen)
	{
		const Gdiplus::Rect rect(x, y, width, height);
		g_pBackBufferGraphics->DrawRectangle(pen, rect);
	}

	// 애니메이션을 할때는 비트맵을 한장씩 넘기기
	void RenderCharacterAnimation(ACharacter::FAnimationBundle& animationBundle, ACharacter* character)
	{
		if (character == nullptr || animationBundle.animationComponent[(int)character->dirState][(int)character->animstate]->m_frames == nullptr)
		{
			printf("RenderCharacterAnimation Error!!!");
			return;
		}
		Gdiplus::Bitmap* pImageBitmap = animationBundle.animationComponent[(int)character->dirState][(int)character->animstate]
			->m_frames[animationBundle.animationComponent[(int)character->dirState][(int)character->animstate]->m_ianimationClip]
			->m_frame;

		FVector2 position = character->GetActorLocation();
		const float rotation = character->GetActorRotation();
		const FVector2 scale = character->GetActorScale();
		const FVector2 size = FVector2(pImageBitmap->GetWidth(), pImageBitmap->GetHeight());
		RenderImage(pImageBitmap, position, rotation, scale, size);
	}

	void RenderTextUI(SUITextComponent* textui, int _x, int _y)
	{
		wchar_t* name = textui->m_content;
		if (name != nullptr)
		{
			FVector2 position = textui->m_Position + FVector2(_x, _y);
			const FVector2 size = textui->m_Size;
			Gdiplus::FontFamily fontFamily(textui->m_fontFamily);
			Gdiplus::Font font(&fontFamily, textui->m_fontSize, textui->m_efontStyle, textui->m_eworldUnit);
			Gdiplus::StringFormat stringFormat;
			stringFormat.SetAlignment(textui->m_fontAlignment);	// 영역의 상단에 맞춰지게 
			stringFormat.SetLineAlignment(textui->m_fontLineAlignment); // 다음 줄로 갱신될 때 왼쪽부터 써지기
			stringFormat.SetTrimming(textui->m_fontTrimming); // trim 안함
			Gdiplus::SolidBrush brush(textui->m_color);
			RenderText(name, position, size, font, stringFormat, brush);
		}
	}
	// 액터 그리기
	void RenderActor(AActor* actor)
	{
		Gdiplus::Bitmap* pImageBitmap = actor->GetBitmap();
		FVector2 position = actor->GetActorLocation();
		const float rotation = actor->GetActorRotation();
		const FVector2 scale = actor->GetActorScale();
		const FVector2 size = actor->GetActorSize();
		RenderImage(pImageBitmap, position, rotation, scale, size);
	}

	void RenderMesh(USceneComponent* sceneComponent, UStaticMeshComponent* staticMesh)
	{
		Gdiplus::Bitmap* pImageBitmap = staticMesh->GetMesh();
		FVector2 position = sceneComponent->GetSceneComponentLocation();
		const float rotation = sceneComponent->GetSceneComponentRotation();
		const FVector2 scale = sceneComponent->GetSceneComponentScale();
		const FVector2 size = staticMesh->GetMeshSize();
		RenderImage(pImageBitmap, position, rotation, scale, size);
	}

	void EndDraw()
	{
		BitBlt(g_FrontBufferDC, 0, 0, (int)g_resolution.x, (int)g_resolution.y, g_BackBufferDC, 0, 0, SRCCOPY);
	}

	void SetResolution(const float& width, const float& height)
	{
		g_resolution.x = width;
		g_resolution.y = height;
	}

	FVector2 GetResolution()
	{
		return g_resolution;
	}

	void SetMainCamera(const std::shared_ptr<ACameraActor> camera)
	{
		g_mainCamera = camera;
	}

	std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return g_mainCamera;
	}
}
