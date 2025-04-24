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
	Renderer::SetResolution(1280, 800);	// 해상도 조절
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
	UScene* g_currentScene = new UMenuScene();
	UScene* g_nextScene = g_currentScene;
	FVector2 m_LMouseCickPosition;
	FVector2 m_RMouseCickPosition;
	AGameStateBase* g_gameInstance = nullptr;

	void PreInitialize()
	{
		g_gameInstance = AGameStateBase::CreateInstance();
		g_gameInstance->Initialize();
		Renderer::SetMainCamera(g_gameInstance->GetMainCamera());
	}

	void Initialize(HWND hwnd)
	{
		Renderer::Initialize(hwnd);
		Time::Initialize();
		Game::GetCurrentScene()->Initialize();
		Game::GetCurrentScene()->LoadData();
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

		//printf("%f\n", 1 / Time::GetElapsedTime());
		Renderer::RenderRectRed(int(m_LMouseCickPosition.x - 10), int(m_LMouseCickPosition.y - 10), 20, 20);
		Renderer::RenderRectBlue(int(m_RMouseCickPosition.x - 10), int(m_RMouseCickPosition.y - 10), 20, 20);

		// Renderer::EndDraw
		Renderer::EndDraw();

		ChangeScene();

		if (Input::IsKeyDown(VK_W))
		{
			FVector2 cameraPos = Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
			cameraPos += FVector2(0, -5);
			Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(cameraPos);
		}

		if (Input::IsKeyDown(VK_S))
		{
			FVector2 cameraPos = Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
			cameraPos += FVector2(0, 5);
			Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(cameraPos);
		}

		if (Input::IsKeyDown(VK_A))
		{
			FVector2 cameraPos = Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
			cameraPos += FVector2(-5, 0);
			Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(cameraPos);
		}
		if (Input::IsKeyDown(VK_D))
		{
			FVector2 cameraPos = Game::GetGameState()->GetMainCamera().get()->GetCameraLocation();
			cameraPos += FVector2(5, 0);
			Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(cameraPos);
		}
	}

	void Release(HWND hwnd)
	{
		Renderer::Release(hwnd);
		g_currentScene->Release();
	}

	UScene* GetCurrentScene()
	{
		return g_currentScene;
	}

	UScene** GetCurrentScenePtr()
	{
		return &g_currentScene;
	}

	UScene* GetNextScene()
	{
		return g_nextScene;
	}

	UScene** GetNextScenePtr()
	{
		return &g_nextScene;
	}

	AGameStateBase* GetGameState()
	{
		return g_gameInstance;
	}

	void ChangeScene()
	{
		if (g_currentScene != g_nextScene)
		{
			g_currentScene->Release();
			delete g_currentScene;
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
}
