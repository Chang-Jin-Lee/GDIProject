// Renderer.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "Renderer.h"
#include "../../Classes/Actor.h"
#include "../../Math/Math.h"
#include "../../Classes/Character.h"
#include "../../UI/UITextComponent.h"
#include "../../UI/UIButtonComponent.h"
#include "../../Classes/Camera/CameraActor.h"
#include "../../Classes/Components/SceneComponent.h"
#include "../../Classes/Components/StaticMeshComponent.h"
#include <iostream>
#include "../../UI/Widget.h"
#include "../../Experiment/SmartCast.h"
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")


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

	std::weak_ptr<ACameraActor> g_mainCamera;
	std::vector<std::weak_ptr<UObject>> g_renderedObjs;

    Gdiplus::Pen* m_redPen = nullptr;
    Gdiplus::Pen* m_BluePen = nullptr;
    Gdiplus::SolidBrush* m_redBrush = nullptr;
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

        if (!m_redPen) m_redPen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0), 1.0f);
        if (!m_BluePen) m_BluePen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 255), 1.0f);
		if (!m_redBrush) m_redBrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0));
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
        ::PatBlt(g_BackBufferDC, 0, 0, (int)g_resolution.x, (int)g_resolution.y, BLACKNESS);
	}

	void EndDraw()
	{
		// 그려진 백버퍼를 화면으로 복사
        ::BitBlt(
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
		for (auto object : g_renderedObjs)
		{
			if (auto obejctRef = object.lock())
			{
				if (std::shared_ptr<ACharacter> character = std::dynamic_pointer_cast<ACharacter>(obejctRef))
				{
                    RenderCharacterAnimation(character.get());
				}
				else if (std::shared_ptr<AActor> actor = std::dynamic_pointer_cast<AActor>(obejctRef))
				{
					if (actor->bVisible)
					{
						if (auto componentRef = actor->SceneComponent.lock())
						{
							if (auto StaticMeshComponentRef = actor->StaticMeshComponent.lock())
							{
                                RenderMesh(componentRef, StaticMeshComponentRef, actor->bSelected);
							}
						}
					}
				}
				else if (std::shared_ptr<UWidget> widget = std::dynamic_pointer_cast<UWidget>(obejctRef))
				{
					for (auto UWidgetComponent : widget->WidgetComponents)
					{
						if (auto UWidgetComponentRef = UWidgetComponent.lock())
						{
							if (UWidgetComponentRef->IsVisible() == false) continue;
							if (UWidgetComponentRef->m_AttachedActor.expired()) // 액터에 부착되어 있지 않을 때
							{
								if (std::shared_ptr<SUITextComponent> uitextComponent = std::dynamic_pointer_cast<SUITextComponent>(UWidgetComponentRef))
								{
                                    Renderer::RenderTextUI(uitextComponent, 0, 0);
								}
								else if (std::shared_ptr<SUIButtonComponent> uitextComponent = std::dynamic_pointer_cast<SUIButtonComponent>(UWidgetComponentRef))
								{
                                    Renderer::RenderButtonUI(uitextComponent, 0, 0);
								}
							}
							else // 어떤 액터에 부착되어 있을 때
							{
                                if (std::shared_ptr<SUITextComponent> uitextComponent = std::dynamic_pointer_cast<SUITextComponent>(UWidgetComponentRef))
                                {
                                    if (auto actor = UWidgetComponentRef->m_AttachedActor.lock())
                                    {
                                        int parentX = (int)actor->GetActorLocation().x;
                                        int parentY = (int)actor->GetActorLocation().y;
                                        if (UWidgetComponentRef->GetWidgetRenderType() == UWidgetComponent::WidgetRenderType::Camera)
                                        {
                                            if (auto cam = g_mainCamera.lock())
                                            {
                                                parentX -= (int)cam->GetCameraLocation().x;
                                                parentY -= (int)cam->GetCameraLocation().y;
                                            }
                                        }
                                        Renderer::RenderTextUI(uitextComponent, parentX, parentY);
                                    }
                                }
                                else if (std::shared_ptr<SUIButtonComponent> uitextComponent = std::dynamic_pointer_cast<SUIButtonComponent>(UWidgetComponentRef))
                                {
                                    if (auto actor = UWidgetComponentRef->m_AttachedActor.lock())
                                    {
                                        int parentX = (int)actor->GetActorLocation().x;
                                        int parentY = (int)actor->GetActorLocation().y;
                                        if (UWidgetComponentRef->GetWidgetRenderType() == UWidgetComponent::WidgetRenderType::Camera)
                                        {
                                            if (auto cam = g_mainCamera.lock())
                                            {
                                                parentX -= (int)cam->GetCameraLocation().x;
                                                parentY -= (int)cam->GetCameraLocation().y;
                                            }
                                        }
                                        Renderer::RenderButtonUI(uitextComponent, parentX, parentY);
                                    }
                                }
							}
						}
					}
				}
			}
		}
		g_renderedObjs.clear();
	}

	void RenderImage(Gdiplus::Bitmap* pImageBitmap, FVector2& position, const float& rotation, const FVector2& scale, const FVector2& size, const bool& bSelected)
	{
               // camera culling (basic)
               if (auto cam = g_mainCamera.lock())
               {
                   if (position.x < cam->GetCameraLocation().x - g_resolution.x * 0.5f ||
                       position.x > cam->GetCameraLocation().x + g_resolution.x * 1.5f ||
                       position.y < cam->GetCameraLocation().y - g_resolution.y * 0.5f ||
                       position.y > cam->GetCameraLocation().y + g_resolution.y * 1.5f)
                   {
                       // off-screen (pre-zoom heuristic)
                   }
               }

               // compute camera-relative position and zoom
               float zoom = 1.0f;
               FVector2 finalPos = position;
               if (auto cam = g_mainCamera.lock())
               {
                   finalPos = position - cam->GetCameraLocation();
                   zoom = cam->GetCameraScale().x;
                   if (zoom < 0.1f) zoom = 0.1f;
               }

               // apply zoom to position and size
               finalPos = finalPos * zoom;
               const int drawW = (int)(size.x * zoom);
               const int drawH = (int)(size.y * zoom);

               g_pBackBufferGraphics->DrawImage(pImageBitmap, (int)finalPos.x, (int)finalPos.y, drawW, drawH);

               if (bSelected)
               {
                   int circleRadius = (int)(12 * zoom);
                   if (circleRadius < 2) circleRadius = 2;
                   Gdiplus::Rect ellipseRect(
                       (int)(finalPos.x + (drawW / 2) - circleRadius),
                       (int)(finalPos.y + (drawH / 2) - circleRadius),
                       circleRadius * 2,
                       circleRadius * 2
                   );
                   g_pBackBufferGraphics->FillEllipse(m_redBrush, ellipseRect);
               }
	}

	void RenderText(const wchar_t* content, FVector2& position, const FVector2& size, const Gdiplus::Font& font, const Gdiplus::StringFormat& stringFormat, const Gdiplus::SolidBrush& brush)
	{
		if (auto g_mainCameraRef = g_mainCamera.lock())
		{
			if (position.x < g_mainCameraRef->GetCameraLocation().x - g_resolution.x * 0.2f ||
				position.x > g_mainCameraRef->GetCameraLocation().x + g_resolution.x * 1.2f ||
				position.y < g_mainCameraRef->GetCameraLocation().y - g_resolution.x * 0.2f ||
				position.y > g_mainCameraRef->GetCameraLocation().y + g_resolution.y * 1.2f
				)
				return;
		}

		FVector2 finalPos = position;
		if (auto g_mainCameraRef = g_mainCamera.lock())
		{
			finalPos = position - g_mainCameraRef->GetCameraLocation();
		}

		Gdiplus::RectF rect(finalPos.x, finalPos.y, size.x, size.y);
		g_pBackBufferGraphics->DrawString(content, (INT)wcslen(content), &font, rect, &stringFormat, &brush);
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

    void RenderRectFill(Gdiplus::SolidBrush& brush, FVector2& position, const FVector2& size, int radius)
	{
               float zoom = 1.0f;
               FVector2 finalPos = position;
               if (auto cam = g_mainCamera.lock())
               {
                   finalPos = position - cam->GetCameraLocation();
                   zoom = cam->GetCameraScale().x;
               }
               finalPos = finalPos * zoom;
               const int x = (int)finalPos.x;
               const int y = (int)finalPos.y;
               const int width = (int)(size.x * zoom);
               const int height = (int)(size.y * zoom);
               g_pBackBufferGraphics->FillRectangle(&brush, x, y, width, height);
	}

    void RenderRectWithRounded(Gdiplus::SolidBrush* brush, FVector2& position, const FVector2& size, int radius)
	{
               float zoom = 1.0f;
               FVector2 finalPos = position;
               if (auto cam = g_mainCamera.lock())
               {
                   finalPos = position - cam->GetCameraLocation();
                   zoom = cam->GetCameraScale().x;
               }
               finalPos = finalPos * zoom;
               const int x = (int)finalPos.x;
               const int y = (int)finalPos.y;
               const int width = (int)(size.x * zoom);
               const int height = (int)(size.y * zoom);

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
	

    void RenderButtonUI(const std::shared_ptr<SUIButtonComponent>& buttonui, int parentX, int parentY)
	{
               FVector2 position = buttonui->GetPosition() + FVector2(parentX, parentY);
               RenderRectWithRoundedUI(buttonui->GetBrush(), position, buttonui->GetSize(), buttonui->GetRadius());
	}

           void RenderRectWithRoundedUI(Gdiplus::SolidBrush* brush, FVector2& position, const FVector2& size, int radius)
           {
               // 카메라 줌 비적용: 화면 고정 좌표로 그대로 렌더링
               const int x = (int)position.x;
               const int y = (int)position.y;
               const int width = (int)size.x;
               const int height = (int)size.y;

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

	void ClearRenderObjects()
	{
		g_renderedObjs.clear();
	}

	void SetRenderObject(std::weak_ptr<UObject> obj)
	{
		g_renderedObjs.push_back(obj);
	}

	// 애니메이션을 할때는 비트맵을 한장씩 넘기기
    void RenderCharacterAnimation(ACharacter* character)
	{
		if (!character)
		{
			printf("RenderCharacterAnimation Error: character is null\n");
			return;
		}

        auto animComp = character->AnimationBundle.animationComponent[(int)character->dirState][(int)character->animstate];
		if (!animComp || animComp->m_frames.empty())
		{
			printf("RenderCharacterAnimation Error: animation component or frames are null\n");
			return;
		}

		const int clipIndex = animComp->m_ianimationClip;
		const int rowIndex = clipIndex;

		// 프레임 존재 여부 확인
		if (rowIndex >= animComp->m_frames.size() || animComp->m_frames[rowIndex].empty())
		{
			printf("RenderCharacterAnimation Error: frame index out of bounds\n");
			return;
		}

		Gdiplus::Bitmap* pImageBitmap = animComp->m_frames[rowIndex][0].m_frame;

		if (!pImageBitmap)
		{
			printf("RenderCharacterAnimation Error: frame image is null\n");
			return;
		}

		FVector2 position = character->GetActorLocation();
		const float rotation = character->GetActorRotation();
		const FVector2 scale = character->GetActorScale();
		const FVector2 size = FVector2(pImageBitmap->GetWidth(), pImageBitmap->GetHeight());

		RenderImage(pImageBitmap, position, rotation, scale, size, character->bSelected);
	}

    void RenderTextUI(const std::shared_ptr<SUITextComponent>& textui, int parentX, int parentY)
	{
		const std::wstring& name = textui->GetContent();
		if (name.empty() == false)
		{
			FVector2 position = textui->GetPosition() + FVector2(parentX, parentY);
			const FVector2 size = textui->GetSize();
			Gdiplus::FontFamily fontFamily(textui->GetFontFamily().c_str());
			Gdiplus::Font font(&fontFamily, textui->GetFontSize(), (Gdiplus::FontStyle)textui->GetFontStyle(), (Gdiplus::Unit)textui->GetWorldUnit());
			Gdiplus::StringFormat stringFormat;
			stringFormat.SetAlignment((Gdiplus::StringAlignment)textui->GetFontAlignment());
			stringFormat.SetLineAlignment((Gdiplus::StringAlignment)textui->GetFontLineAlignment());
			stringFormat.SetTrimming((Gdiplus::StringTrimming)textui->GetFontTrimming());
			Gdiplus::SolidBrush brush(textui->GetColor());
			RenderText(name.c_str(), position, size, font, stringFormat, brush);
		}
	}

    void RenderMesh(const std::shared_ptr<USceneComponent>& sceneComponent, const std::shared_ptr<UStaticMeshComponent>& staticMesh, const bool& bSelected)
	{
        Gdiplus::Bitmap* pImageBitmap = staticMesh->GetMesh();
		FVector2 position = sceneComponent->GetSceneComponentLocation();
		const float rotation = sceneComponent->GetSceneComponentRotation();
		const FVector2 scale = sceneComponent->GetSceneComponentScale();
		const FVector2 size = staticMesh->GetMeshSize();
		RenderImage(pImageBitmap, position, rotation, scale, size, bSelected);
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

	void SetMainCamera(const std::weak_ptr<ACameraActor> camera)
	{
		if (const auto cameraRef = camera.lock())
		{
			g_mainCamera = cameraRef;
		}
	}

	bool IsGdiValid()
	{
		return IsGdiActive;
	}

	std::weak_ptr<ACameraActor> GetMainCamera()
	{
		return g_mainCamera;
	}
}
