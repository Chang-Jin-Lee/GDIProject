// Renderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Renderer.h"
#include "../../Classes/Actor.h"
#include "../../Math/Math.h"
#include "../../Classes/Camera/CameraActor.h"
#include <iostream>
#include "../../UI/Widget.h"


namespace Renderer
{
	FVector2 g_resolution = FVector2(1280, 800);
	HWND g_hWnd;
	HDC g_FrontBufferDC;    // 앞면 DC
	HDC g_BackBufferDC;     // 뒷면 DC (공유)

	HBITMAP g_BackBufferBitmapA; // 더블 버퍼 A
	HBITMAP g_BackBufferBitmapB; // 더블 버퍼 B
	bool g_bUsingBufferA = true; // 현재 A를 사용하고 있는가

	ULONG_PTR g_GdiPlusToken;
	Gdiplus::Graphics* g_pBackBufferGraphics = nullptr;
	bool IsGdiActive = false;

	std::shared_ptr<ACameraActor> g_mainCamera;
	std::vector<std::shared_ptr<UObject>> g_renderedObjs;

	const Gdiplus::Pen* m_redPen = nullptr;
	const Gdiplus::Pen* m_BluePen = nullptr;

	void Initialize(HWND hwnd)
	{
		g_hWnd = hwnd;
		g_FrontBufferDC = GetDC(hwnd);
		g_BackBufferDC = CreateCompatibleDC(g_FrontBufferDC);

		g_BackBufferBitmapA = CreateCompatibleBitmap(g_FrontBufferDC, (int)g_resolution.x, (int)g_resolution.y);
		g_BackBufferBitmapB = CreateCompatibleBitmap(g_FrontBufferDC, (int)g_resolution.x, (int)g_resolution.y);
		SelectObject(g_BackBufferDC, g_BackBufferBitmapA); // 초기 선택

		Gdiplus::GdiplusStartupInput gsi;
		Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
		IsGdiActive = true;

		g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);
		g_pBackBufferGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
		g_pBackBufferGraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		g_pBackBufferGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);

		if (!m_redPen) m_redPen = new Gdiplus::Pen(Color(255, 0, 0), 1.0f);
		if (!m_BluePen) m_BluePen = new Gdiplus::Pen(Color(0, 0, 255), 1.0f);
	}

	void BeginDraw()
	{
		// 프레임마다 백버퍼 교대
		HBITMAP currentBitmap = g_bUsingBufferA ? g_BackBufferBitmapA : g_BackBufferBitmapB;
		SelectObject(g_BackBufferDC, currentBitmap);

		// g_pBackBufferGraphics를 새로 갱신
		if (g_pBackBufferGraphics)
			delete g_pBackBufferGraphics;
		g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);
		g_pBackBufferGraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
		g_pBackBufferGraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		g_pBackBufferGraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);

		// 화면 지우기
		PatBlt(g_BackBufferDC, 0, 0, (int)g_resolution.x, (int)g_resolution.y, BLACKNESS);
	}

	void EndDraw()
	{
		// 그려진 백버퍼를 화면으로 복사
		BitBlt(
			g_FrontBufferDC,
			0, 0, (int)g_resolution.x, (int)g_resolution.y,
			g_BackBufferDC,
			0, 0,
			SRCCOPY
		);

		// 다음 프레임에 버퍼를 교대
		g_bUsingBufferA = !g_bUsingBufferA;
	}

	void Release(HWND hwnd)
	{
		DeleteObject(g_BackBufferBitmapA);
		DeleteObject(g_BackBufferBitmapB);
		DeleteDC(g_BackBufferDC);
		ReleaseDC(hwnd, g_FrontBufferDC);

		if (g_pBackBufferGraphics)
			delete g_pBackBufferGraphics;

		IsGdiActive = false;
		Gdiplus::GdiplusShutdown(g_GdiPlusToken);
	}


	void Update()
	{
		for (const auto& object : g_renderedObjs)
		{
			if (std::shared_ptr<ACharacter> character = std::dynamic_pointer_cast<ACharacter>(object))
			{
				RenderCharacterAnimation(character->AnimationBundle, character.get());
			}
			else if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(object))
			{
				RenderMesh(actor.get()->SceneComponent, actor.get()->StaticMeshComponent);
			}
			else if (std::shared_ptr<UWidget> widget = std::dynamic_pointer_cast<UWidget>(object))
			{
				for (const auto& UWidgetComponent : widget->WidgetComponents)
				{
					if (UWidgetComponent->IsVisible() == false) continue;
					if (UWidgetComponent->m_AttachedActor == nullptr) // 액터에 부착되어 있지 않을 때
					{
						if (std::shared_ptr<SUITextComponent> uitextComponent = std::dynamic_pointer_cast<SUITextComponent>(UWidgetComponent))
						{
							Renderer::RenderTextUI(uitextComponent, 0, 0);
						}
						else if (std::shared_ptr<SUIButtonComponent> uitextComponent = std::dynamic_pointer_cast<SUIButtonComponent>(UWidgetComponent))
						{
							Renderer::RenderButtonUI(uitextComponent, 0, 0);
						}
					}
					else // 어떤 액터에 부착되어 있을 때
					{
						if (std::shared_ptr<SUITextComponent> uitextComponent = std::dynamic_pointer_cast<SUITextComponent>(UWidgetComponent))
						{
							Renderer::RenderTextUI(uitextComponent, UWidgetComponent->m_AttachedActor->GetActorLocation().x, UWidgetComponent->m_AttachedActor->GetActorLocation().y);
						}
						else if (std::shared_ptr<SUIButtonComponent> uitextComponent = std::dynamic_pointer_cast<SUIButtonComponent>(UWidgetComponent))
						{
							Renderer::RenderButtonUI(uitextComponent, UWidgetComponent->m_AttachedActor->GetActorLocation().x, UWidgetComponent->m_AttachedActor->GetActorLocation().y);
						}
					}
				}
			}
		}
	}

	void RenderImage(Gdiplus::Bitmap* pImageBitmap, FVector2& position, const float& rotation, const FVector2& scale, const FVector2& size)
	{
		if (position.x < g_mainCamera->GetCameraLocation().x - g_resolution.x * 0.2f ||
			position.x > g_mainCamera->GetCameraLocation().x + g_resolution.x * 1.2f ||
			position.y < g_mainCamera->GetCameraLocation().y - g_resolution.x * 0.2f ||
			position.y > g_mainCamera->GetCameraLocation().y + g_resolution.y * 1.2f
			)
			return;

		const int imgWidth = size.x;
		const int imgHeight = size.y;

		FVector2 finalPos = position;
		if (g_mainCamera)
			finalPos = position - g_mainCamera->GetCameraLocation();

		//g_pBackBufferGraphics->DrawImage(pImageBitmap, (int)finalPos.x, (int)finalPos.y, (int)size.x, (int)size.y);
		g_pBackBufferGraphics->DrawImage(pImageBitmap, (int)finalPos.x, (int)finalPos.y, (int)size.x, (int)size.y);

		//if (pImageBitmap && g_pBackBufferGraphics)
		//{
		//	// 회전 0, 스케일 1이면 Transform 안하고 바로 Draw
		//	if (rotation == 0.0f && scale.x == 1.0f && scale.y == 1.0f)
		//	{
		//		g_pBackBufferGraphics->DrawImage(pImageBitmap, (int)finalPos.x, (int)finalPos.y, (int)size.x, (int)size.y);
		//	}
		//	else
		//	{
		//		// 변형이 필요한 경우만 Transform 적용
		//		//Gdiplus::PointF center = Gdiplus::PointF(float(size.x / 2), float(size.y / 2));
		//		//Gdiplus::Matrix matrix;
		//		//matrix.Translate((float)finalPos.x, (float)finalPos.y);
		//		//matrix.Scale(scale.x, scale.y);
		//		//matrix.RotateAt(rotation, center);
		//		//g_pBackBufferGraphics->SetTransform(&matrix);
		//		//g_pBackBufferGraphics->DrawImage(pImageBitmap, 0, 0, (int)size.x, (int)size.y);
		//		//g_pBackBufferGraphics->ResetTransform();
		//	}
		//}
	}

	void RenderText(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush)
	{
		if (position.x < g_mainCamera->GetCameraLocation().x - g_resolution.x * 0.2f ||
			position.x > g_mainCamera->GetCameraLocation().x + g_resolution.x * 1.2f ||
			position.y < g_mainCamera->GetCameraLocation().y - g_resolution.x * 0.2f ||
			position.y > g_mainCamera->GetCameraLocation().y + g_resolution.y * 1.2f
			)
			return;

		FVector2 finalPos = position;
		if (g_mainCamera)
			finalPos = position - g_mainCamera->GetCameraLocation();

		RenderRectRed(finalPos.x, finalPos.y, (int)size.x, (int)size.y);
		Gdiplus::RectF rect(finalPos.x, finalPos.y, size.x, size.y);
		g_pBackBufferGraphics->DrawString(content, (INT)wcslen(content), &font, rect, &stringFormat, &brush);

		//Gdiplus::PointF center = Gdiplus::PointF(float(size.x / 2), float(size.y / 2));
		//Gdiplus::Matrix matrix;
		//matrix.Translate(finalPos.x, finalPos.y);
		//matrix.Scale(1.0f, 1.0f);
		//matrix.RotateAt(0, center);
		//g_pBackBufferGraphics->SetTransform(&matrix); // 매트릭스 적용.
		//RenderRectRed(0, 0, (int)size.x, (int)size.y);
		//g_pBackBufferGraphics->DrawString(content, (INT)wcslen(content), &font, Gdiplus::RectF(0, 0, size.x, size.y), &stringFormat, &brush);
		//g_pBackBufferGraphics->ResetTransform();
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

	void RenderRectFill(SolidBrush& brush, FVector2& position, const FVector2& size, int radius)
	{
		//Gdiplus::SolidBrush brush(Gdiplus::Color(180, 200, 220));
		if (position.x < g_mainCamera->GetCameraLocation().x - g_resolution.x * 0.2f ||
			position.x > g_mainCamera->GetCameraLocation().x + g_resolution.x * 1.2f ||
			position.y < g_mainCamera->GetCameraLocation().y - g_resolution.x * 0.2f ||
			position.y > g_mainCamera->GetCameraLocation().y + g_resolution.y * 1.2f
			)
			return;

		FVector2 finalPos = position;
		if (g_mainCamera)
			finalPos = position - g_mainCamera->GetCameraLocation();
		int x = position.x;
		int y = position.y;
		int width = size.x;
		int height = size.y;
		g_pBackBufferGraphics->FillRectangle(&brush, x, y, width, height);
	}

	void RenderRectWithRounded(SolidBrush* brush, FVector2& position, const FVector2& size, int radius)
	{
		FVector2 finalPos = position;
		if (g_mainCamera)
			finalPos = position - g_mainCamera->GetCameraLocation();

		int x = finalPos.x;
		int y = finalPos.y;
		int width = size.x;
		int height = size.y;

		Gdiplus::PointF center = Gdiplus::PointF(width / 2.0f, height / 2.0f);
		Gdiplus::Matrix matrix;
		matrix.Translate((float)x, (float)y);
		matrix.RotateAt(0, center);
		g_pBackBufferGraphics->SetTransform(&matrix);

		Gdiplus::GraphicsPath path;
		path.AddArc(0, 0, radius * 2, radius * 2, 180, 90);
		path.AddLine(radius, 0, width - radius, 0);
		path.AddArc(width - radius * 2, 0, radius * 2, radius * 2, 270, 90);
		path.AddLine(width, radius, width, height - radius);
		path.AddArc(width - radius * 2, height - radius * 2, radius * 2, radius * 2, 0, 90);
		path.AddLine(width - radius, height, radius, height);
		path.AddArc(0, height - radius * 2, radius * 2, radius * 2, 90, 90);
		path.AddLine(0, height - radius, 0, radius);
		path.CloseFigure();

		g_pBackBufferGraphics->FillPath(brush, &path);
		g_pBackBufferGraphics->ResetTransform();
	}
	

	void RenderButtonUI(std::shared_ptr<SUIButtonComponent>& buttonui, int parentX, int parentY)
	{
		FVector2 position = buttonui->m_Position + FVector2(parentX, parentY);
		RenderRectWithRounded(buttonui->m_brush, position, buttonui->m_Size, buttonui->m_radius);
	}

	void ClearRenderObjects()
	{
		g_renderedObjs.clear();
	}

	void SetRenderObject(std::shared_ptr<UObject> obj)
	{
		g_renderedObjs.push_back(obj);
	}

	// 애니메이션을 할때는 비트맵을 한장씩 넘기기
	void RenderCharacterAnimation(ACharacter::FAnimationBundle& animationBundle, ACharacter* character)
	{
		if (character == nullptr || animationBundle.animationComponent[(int)character->dirState][(int)character->animstate]->m_frames == nullptr)
		{
			printf("RenderCharacterAnimation Error!!!");
			return;
		}
		Gdiplus::Bitmap*& pImageBitmap = animationBundle.animationComponent[(int)character->dirState][(int)character->animstate]
			->m_frames[animationBundle.animationComponent[(int)character->dirState][(int)character->animstate]->m_ianimationClip]
			->m_frame;

		FVector2 position = character->GetActorLocation();
		const float rotation = character->GetActorRotation();
		const FVector2 scale = character->GetActorScale();
		const FVector2 size = FVector2(pImageBitmap->GetWidth(), pImageBitmap->GetHeight());
		RenderImage(pImageBitmap, position, rotation, scale, size);
	}

	void RenderTextUI(std::shared_ptr<SUITextComponent>& textui, int parentX, int parentY)
	{
		std::wstring& name = textui->m_content;
		if (name.empty() == false)
		{
			FVector2 position = textui->m_Position + FVector2(parentX, parentY);
			const FVector2 size = textui->m_Size;
			Gdiplus::FontFamily fontFamily(textui->m_fontFamily.c_str());
			Gdiplus::Font font(&fontFamily, textui->m_fontSize, textui->m_efontStyle, textui->m_eworldUnit);
			Gdiplus::StringFormat stringFormat;
			stringFormat.SetAlignment(textui->m_fontAlignment);	// 영역의 상단에 맞춰지게 
			stringFormat.SetLineAlignment(textui->m_fontLineAlignment); // 다음 줄로 갱신될 때 왼쪽부터 써지기
			stringFormat.SetTrimming(textui->m_fontTrimming); // trim 안함
			Gdiplus::SolidBrush brush(textui->m_color);
			RenderText(name.c_str(), position, size, font, stringFormat, brush);
		}
	}

	void RenderMesh(std::shared_ptr<USceneComponent>& sceneComponent, std::shared_ptr<UStaticMeshComponent>& staticMesh)
	{
		Gdiplus::Bitmap*&& pImageBitmap = std::move(staticMesh->GetMesh());
		FVector2 position = sceneComponent->GetSceneComponentLocation();
		const float rotation = sceneComponent->GetSceneComponentRotation();
		const FVector2 scale = sceneComponent->GetSceneComponentScale();
		const FVector2 size = staticMesh->GetMeshSize();
		RenderImage(pImageBitmap, position, rotation, scale, size);
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

	bool IsGdiValid()
	{
		return IsGdiActive;
	}

	std::shared_ptr<ACameraActor> GetMainCamera()
	{
		return g_mainCamera;
	}
}
