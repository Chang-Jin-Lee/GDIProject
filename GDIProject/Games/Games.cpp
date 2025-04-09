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
#include "PlayerCharacter.h"
#include <Classes/Scene/Scene.h>
#include "Scene/MenuScene.h"
#include "Scene/PlayScene.h"
#include "Games.h"
#include <iostream>

LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");

int m_centerX = 0;
int m_centerY = 0;
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

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hwnd, &ps);

		//const char* text = "윈도우 메시지출력 중";
		//TextOutA(hDC, 10, 10, text, (int)strlen(text));
		//
		//const int CELL_SIZE = 300;     // 셀 크기
		//const int PADDING = 10;       // 셀 내부 여백
		//int cols = Renderer::GetWidth() / CELL_SIZE;
		//int rows = Renderer::GetHeight() / CELL_SIZE;
		//
		//RECT rect = { 0, 0, Renderer::GetWidth(), Renderer::GetHeight() };
		//for (int y = 0; y < rows; ++y)
		//{
		//	for (int x = 0; x < cols; ++x)
		//	{
		//		int left = x * CELL_SIZE + PADDING;
		//		int top = y * CELL_SIZE + PADDING;
		//		int right = (x + 1) * CELL_SIZE - PADDING;
		//		int bottom = (y + 1) * CELL_SIZE - PADDING;
		//		Ellipse(hDC, left, top, right, bottom);
		//	}
		//}

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		FillRect(hDC, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

		COLORREF color = RGB(255, 0, 0);
		HPEN hPen = CreatePen(PS_SOLID, 2, color);
		HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
		// 브러시는 내부 채우지 않도록 NULL_BRUSH 사용
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

		Ellipse(hDC,
			m_centerX - m_radius, m_centerY - m_radius,
			m_centerX + m_radius, m_centerY + m_radius);

		// 이전 객체 복원 및 펜 삭제
		SelectObject(hDC, hOldPen);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hPen);

		EndPaint(hwnd, &ps);
		printf("WM_PAINT: 화면 다시 그리기\n");
	}
	break;

	case WM_KEYDOWN:
		printf("WM_KEYDOWN: VK_CODE = %d\n", (int)wParam);
		break;

	case WM_CHAR:
		printf("WM_CHAR: 문자 입력 = '%c'\n", (char)wParam);
		break;

	case WM_LBUTTONDOWN:
		printf("WM_LBUTTONDOWN: 클릭 위치 x=%d y=%d\n", LOWORD(lParam), HIWORD(lParam));
		//std::cout << "WM_LBUTTONDBLCLK" << std::endl;
		// 마우스 왼쪽 버튼 더블 클릭
		// 좌표를 가져온다.
		//int x = LOWORD(lparam);
		//int y = HIWORD(lparam);

		m_centerX = LOWORD(lParam);
		m_centerY = HIWORD(lParam);
		//simplegeo::g_GeoShapeManager.AddCircle(x, y, 10, RGB(255, 0, 0));
		// 펜 생성 및 선택

		::InvalidateRect(hwnd, NULL, TRUE);

		break;

		// 메세지가 너무 자주 나오는것은 출력하지 않음
	case WM_MOUSEMOVE:
	case WM_NCHITTEST:
	case WM_NCMOUSEMOVE:
	case WM_SETCURSOR:
		break;

	case WM_DESTROY:
		printf("WM_DESTROY: 프로그램 종료\n");
		PostQuitMessage(0);
		break;

	default:
		//printf("%s (0x%04X)\n", GetMessageName(msg), msg);
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
	RECT rcClient = { 0, 0, (LONG)Renderer::GetWidth(), (LONG)Renderer::GetHeight() };
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

		// Renderer::EndDraw
		Renderer::EndDraw();

		ChangeScene();
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


	void ChangeScene()
	{
		if (g_currentScene != g_nextScene)
		{
			g_currentScene->Release();
			delete g_currentScene;
			g_currentScene = g_nextScene;
			g_currentScene->Initialize();
			g_currentScene->LoadData();
		}
	}
}
