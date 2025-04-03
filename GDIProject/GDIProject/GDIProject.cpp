#include <windows.h>
#include <stdio.h>

#include <gdiplus.h>
#include "Runtime/Core/Time.h"
#include "Runtime/Baseh/BaseDefine.h"
#pragma comment(lib, "gdiplus.lib")

LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");
//int g_width = 1024;
//int g_height = 768;
//int g_width = 1920;
//int g_height = 1080;
int g_width = 1280;
int g_height = 800;

HWND g_hWnd;
HDC g_FrontBufferDC;    // 앞면 DC
HDC g_BackBufferDC;    // 뒷면 DC
HBITMAP g_BackBufferBitmap;
enum ECharacterName g_eCurrentCharacter = boy;

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
		const char* text = "윈도우 메시지출력 중";
		TextOutA(hDC, 10, 10, text, (int)strlen(text));

		const int CELL_SIZE = 300;     // 셀 크기
		const int PADDING = 10;       // 셀 내부 여백
		int cols = g_width / CELL_SIZE;
		int rows = g_height / CELL_SIZE;

		RECT rect = { 0, 0, g_width, g_height };
		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < cols; ++x)
			{
				int left = x * CELL_SIZE + PADDING;
				int top = y * CELL_SIZE + PADDING;
				int right = (x + 1) * CELL_SIZE - PADDING;
				int bottom = (y + 1) * CELL_SIZE - PADDING;
				Ellipse(hDC, left, top, right, bottom);
			}
		}

		EndPaint(hwnd, &ps);
		printf("WM_PAINT: 화면 다시 그리기\n");
	}
	break;

	case WM_KEYDOWN:
		
		printf("WM_KEYDOWN: VK_CODE = %d\n", (int)wParam);
		if (wParam == VK_1)
		{
			g_eCurrentCharacter = run;
		}
		if (wParam == VK_2)
		{
			g_eCurrentCharacter = attack;
		}
		if (wParam == VK_3)
		{
			g_eCurrentCharacter = run;
		}
		break;

	case WM_CHAR:
		printf("WM_CHAR: 문자 입력 = '%c'\n", (char)wParam);
		break;

	case WM_LBUTTONDOWN:
		printf("WM_LBUTTONDOWN: 클릭 위치 x=%d y=%d\n", LOWORD(lParam), HIWORD(lParam));
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
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
	RECT rcClient = { 0, 0, (LONG)g_width, (LONG)g_height };
	AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

	//생성
	HWND hwnd = CreateWindow(
		g_szClassName,
		TEXT("윈도우 타이틀바에 표시할 문자열"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	////////Renderer::Initialize
	g_FrontBufferDC = GetDC(hwnd); //윈도우 클라이언트 영역의 DeviceContext얻기
	g_BackBufferDC = CreateCompatibleDC(g_FrontBufferDC); // 호환되는 DeviceContext 생성
	g_BackBufferBitmap = CreateCompatibleBitmap(g_FrontBufferDC, g_width, g_height); // 메모리 영역생성
	SelectObject(g_BackBufferDC, g_BackBufferBitmap); // MemDC의 메모리영역 지정

	// GDI+ 초기화
	ULONG_PTR g_GdiPlusToken;
	Gdiplus::GdiplusStartupInput gsi;
	Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gsi, nullptr);
	Gdiplus::Graphics* g_pBackBufferGraphics = Gdiplus::Graphics::FromHDC(g_BackBufferDC);
	Gdiplus::Bitmap* g_pImageBitmap[MAX_CHARACTER_SIZE][MAX_VIDEO_SIZE];
	UINT witdh[MAX_VIDEO_SIZE] = { 0, };
	UINT height[MAX_VIDEO_SIZE] = { 0, };
	int m_iplayidx = 0;
	int m_iplayMax = 0;
	

	for (int j = 0; j < MAX_CHARACTER_SIZE; j++)
	{
		for (int i = 0; i < MAX_VIDEO_SIZE; i++)
		{
			wchar_t wcsbuf[200];
			int     num;
			//num = swprintf(wcsbuf, sizeof(wcsbuf), L"../Resource/frames/frame_%04d.png", i + 1);
			num = swprintf(wcsbuf, sizeof(wcsbuf), L"../Resource/캐릭터%d/pc_%s_0_spr_0.png", j+1, i + 1);

			//g_pImageBitmap[i] = new Gdiplus::Bitmap((WCHAR*)filename);
			g_pImageBitmap[j][i] = new Gdiplus::Bitmap(wcsbuf);
			witdh[i] = g_pImageBitmap[j][i]->GetWidth();
			height[i] = g_pImageBitmap[j][i]->GetHeight();
			if (g_pImageBitmap[j][i]->GetLastStatus() != Gdiplus::Ok)
			{
				m_iplayMax = i;
				break;
				MessageBox(hwnd, L"PNG 파일 로드 실패", L"오류", MB_ICONERROR);
				PostQuitMessage(0);
				m_iplayMax = i;
				break;
			}
		}

	}

	/// 시간 관련
	Time::Initialize();
	float m_fFPSTime = 1 / 30;
	float m_fcountOneSecond = 0;
	m_fcountOneSecond = Time::GetTotalTime();

	MSG msg;
	while (true)
	{
		Time::UpdateTime();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (Time::GetTotalTime() - m_fcountOneSecond >= m_fFPSTime)	// 0.5초에 한 번씩
		{
			m_iplayidx = (m_iplayidx + 1) % m_iplayMax;
			m_fcountOneSecond = Time::GetTotalTime();
		}


		// Renderer::BeginDraw()
		PatBlt(g_BackBufferDC, 0, 0, g_width, g_height, BLACKNESS);

		// Render()
		g_pBackBufferGraphics->DrawImage(g_pImageBitmap[g_eCurrentCharacter][m_iplayidx], (int)(g_width / 2 - witdh[m_iplayidx] / 2), (int)(g_height / 2 - height[m_iplayidx] / 2), witdh[m_iplayidx], height[m_iplayidx]);
		//g_pBackBufferGraphics->DrawImage(g_pImageBitmap, 0, 0, 400, 300);

		// Renderer::EndDraw()
		BitBlt(g_FrontBufferDC, 0, 0, g_width, g_height, g_BackBufferDC, 0, 0, SRCCOPY);
	}

	// Renderer::Uninitialize

	// GDI+ 해제
	for(int j = 0 ; j < MAX_CHARACTER_SIZE; j++)
		for(int i = 0 ; i < m_iplayMax; i++)
			delete g_pImageBitmap[j][i];
	delete g_pBackBufferGraphics;
	Gdiplus::GdiplusShutdown(g_GdiPlusToken);

	DeleteObject(g_BackBufferBitmap);
	DeleteDC(g_BackBufferDC);
	ReleaseDC(hwnd, g_FrontBufferDC);
	//////////////////////////////////////////////////////////////////////////

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;
}
