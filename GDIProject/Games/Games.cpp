#ifdef MEMORY_LEAK_CHECK
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <Input/Input.h>
#include <Time/Time.h>
#include "Character/PlayerCharacter.h"
#include <Classes/Scene/Scene.h>
#include "Scene/MenuScene.h"
#include "Scene/PlayScene.h"
#include "Games.h"
#include <iostream>

LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");

int m_radius = 20;

// 콘솔 초기화
void InitConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitle(L"윈도우 메시지 콘솔 로그");

	printf("콘솔 로그 시작...\n\n");
}

void UninitConsole()
{
	// 표준 출력 스트림 닫기
	fclose(stdout);
	// 콘솔 해제
	FreeConsole();
}

// WIN32 API 에러 값에 대한 실제 메세지를 출력하는 함수
void PrintLastErrorMessage()
{
	DWORD errorCode = GetLastError();
	LPVOID lpMsgBuf;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 기본 언어
		(LPSTR)&lpMsgBuf,
		0,
		NULL);

	if (lpMsgBuf)
	{
		printf("오류 코드: %lu\n오류 메시지: %s\n", errorCode, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf); // 할당된 버퍼 해제
	}
	else
	{
		printf("오류 코드: %lu (메시지를 찾을 수 없음)\n", errorCode);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		printf("WM_CREATE: 윈도우가 생성되었습니다.\n");
		break;
	case WM_LBUTTONDOWN:
		Game::SetLMouseClickPosition(FVector2(LOWORD(lParam), HIWORD(lParam)));
		Game::SetMouseDragState(true);
		Game::OnWidgetClick(FVector2(LOWORD(lParam), HIWORD(lParam)));
		{
			FVector2 CameraPosition = Game::GetGameState()->GetMainCamera().get()->GetActorLocation();
			FVector2 index = ATile::GetIndexAtPosition(FVector2(LOWORD(lParam), HIWORD(lParam)) + CameraPosition);
			FVector2 pos = ATile::GetTilePositionAtIndex(index.x, index.y);
			std::cout << "WndProc index : " << index.x << "  " << index.y << '\n';
			std::cout << "WndProc position : " << pos.x << ' ' << pos.y << '\n';
		}
		break;
	case WM_MOUSEMOVE:
		if (Game::GetMouseDragState())
		{
			FVector2 currentPos = FVector2(LOWORD(lParam), HIWORD(lParam));
			FVector2 dif = currentPos - Game::GetLMouseClickPosition();

			// 카메라 위치 이동
			FVector2 cameraPos = Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
			cameraPos += FVector2(-dif.x, -dif.y);
			Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(cameraPos);
			Game::SetLMouseClickPosition(currentPos);
			//std::cout << "WndProc currentPos : " << currentPos.x << ' ' << currentPos.y << '\n';
		}
		break;
	case WM_LBUTTONUP:
		Game::SetMouseDragState(false);
		ReleaseCapture(); // 마우스 캡처 해제
		break;

	case WM_RBUTTONDOWN:
		Game::SetRMouseClickPosition(FVector2(LOWORD(lParam), HIWORD(lParam))); // 마우스 왼쪽 버튼 더블 클릭
		break;
	case WM_DESTROY:
		printf("WM_DESTROY: 프로그램 종료\n");
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

// 메인 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
#ifdef MEMORY_LEAK_CHECK
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//Renderer::SetResolution(800, 600);	// 해상도 조절
	//Renderer::SetResolution(1280, 800);	// 해상도 조절
	Renderer::SetResolution(1024, 600);	// 해상도 조절
	InitConsole();  // 콘솔 출력 초기화

	char szPath[MAX_PATH] = { 0, };
	::GetCurrentDirectoryA(MAX_PATH, szPath);
	printf("Current Directory: %s\n", szPath);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = g_szClassName;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// 기본 커서 모양
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// 기본 아이콘 모양
	RegisterClass(&wc);

	// 원하는 크기가 조정되어 리턴
	RECT rcClient = { 0, 0, (LONG)Renderer::GetResolution().x, (LONG)Renderer::GetResolution().y };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
	HWND hwnd = CreateWindow(
		g_szClassName,
		TEXT("게임게임!"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	////////Renderer::Initialize
	Game::PreInitialize();
	Game::Initialize(hwnd);
	Game::PostInitialize();

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game::Update();
	}

	Game::Release(hwnd);
	//////////////////////////////////////////////////////////////////////////

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;

#ifdef MEMORY_LEAK_CHECK
	_CrtDumpMemoryLeaks();
#endif
}

namespace Game
{
	std::shared_ptr<UScene> g_currentScene = std::make_shared<UMenuScene>();
	std::shared_ptr<UScene> g_nextScene = g_currentScene;

	GameStateBase* g_gameInstance = nullptr;
	
	// 마우스 드래그
	FVector2 m_LMouseCickPosition;
	FVector2 m_RMouseCickPosition;
	bool g_bMouseDragging = false;
	double m_FPSPerformancetime = 0;

	void PreInitialize()
	{
		g_gameInstance = GameStateBase::CreateInstance();
		g_gameInstance->Initialize();
		Renderer::SetMainCamera(g_gameInstance->GetMainCamera());
	}

	void Initialize(HWND hwnd)
	{
		Renderer::Initialize(hwnd);
		Input::Initialize(hwnd);
		Time::Initialize();
		Game::GetCurrentScene()->Initialize();
		Game::GetCurrentScene()->LoadData();
		m_FPSPerformancetime = Time::GetTotalTime();
	}

	void PostInitialize()
	{
		g_gameInstance->PostInitialize();
	}

	void LoadData()
	{

	}

	void Update()
	{
		Time::UpdateTime();
		Input::Update();
		// Renderer::BeginDraw
		Renderer::BeginDraw();

		g_currentScene->Update();
		g_currentScene->DeleteNullObjects();

		Renderer::Update();

		if (Time::GetTotalTime() - m_FPSPerformancetime > 2)
		{
			printf("%f\n", 1 / Time::GetElapsedTime());
			m_FPSPerformancetime = Time::GetTotalTime();
		}
		Renderer::RenderRectRed(int(m_LMouseCickPosition.x - 10), int(m_LMouseCickPosition.y - 10), 20, 20);
		Renderer::RenderRectBlue(int(m_RMouseCickPosition.x - 10), int(m_RMouseCickPosition.y - 10), 20, 20);

		// Renderer::EndDraw
		Renderer::EndDraw();

		ChangeScene();
	}

	void Release(HWND hwnd)
	{
		Renderer::Release(hwnd);
		g_currentScene->Release();
	}

	void OnWidgetClick(const FVector2& clickPosition)
	{
		std::vector<std::unordered_map<std::wstring, std::shared_ptr<UWidget>>>& Widgets = g_currentScene->GetWidgets();
		for (auto& button : Widgets[static_cast<int>(EUILAYER::HUD)])
		{
			if (button.second->bVisible)
			{
				for (auto& component : button.second.get()->WidgetComponents)
				{
					if (clickPosition.x < component.get()->m_Position.x ||
						clickPosition.x > component.get()->m_Position.x + component.get()->m_Size.x ||
						clickPosition.y < component.get()->m_Position.y ||
						clickPosition.y > component.get()->m_Position.y + component.get()->m_Size.y
						)
						continue;
					if (component->FVoidDelegate)
					{
						component->FVoidDelegate();
					}
				}
			}
		}
	}

	bool CheckWidgetPosition(const FVector2& clickPosition)
	{
		std::vector<std::unordered_map<std::wstring, std::shared_ptr<UWidget>>>& Widgets = g_currentScene->GetWidgets();
		bool bWidgetExist = false;
		for (auto& button : Widgets[static_cast<int>(EUILAYER::HUD)])
		{
			if (button.second->bVisible)
			{
				for (auto& component : button.second.get()->WidgetComponents)
				{
					if ((clickPosition.x < component.get()->m_Position.x ||
						clickPosition.x > component.get()->m_Position.x + component.get()->m_Size.x ||
						clickPosition.y < component.get()->m_Position.y ||
						clickPosition.y > component.get()->m_Position.y + component.get()->m_Size.y) == false
						)
					if(component->m_bVisible == true)
						bWidgetExist = true;
				}
			}
		}
		return bWidgetExist;
	}

	std::shared_ptr<UScene> GetCurrentScene()
	{
		return g_currentScene;
	}

	std::shared_ptr<UScene>* GetCurrentScenePtr()
	{
		return &g_currentScene;
	}

	std::shared_ptr<UScene> GetNextScene()
	{
		return g_nextScene;
	}

	std::shared_ptr<UScene>* GetNextScenePtr()
	{
		return &g_nextScene;
	}

	GameStateBase* GetGameState()
	{
		return g_gameInstance;
	}

	void ChangeScene()
	{
		if (g_currentScene != g_nextScene)
		{
			g_currentScene->Release();
			g_currentScene.reset();
			g_currentScene = g_nextScene;
		}
	}

	FVector2 GetLMouseClickPosition() { return m_LMouseCickPosition; }
	FVector2 GetRMouseClickPosition() { return m_RMouseCickPosition; }
	void SetLMouseClickPosition(const FVector2& rect)
	{
		m_LMouseCickPosition.x = rect.x;
		m_LMouseCickPosition.y = rect.y;
	}
	void SetRMouseClickPosition(const FVector2& rect)
	{
		m_RMouseCickPosition.x = rect.x;
		m_RMouseCickPosition.y = rect.y;
	}

	bool GetMouseDragState()
	{
		return g_bMouseDragging;
	}

	void SetMouseDragState(const bool& state)
	{
		if (std::shared_ptr<UPlayScene> scene = std::dynamic_pointer_cast<UPlayScene>(g_currentScene))
		{
			g_bMouseDragging = state;
		}
	}
}
