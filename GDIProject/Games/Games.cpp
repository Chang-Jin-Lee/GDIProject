#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <Runtime/Renderer/Renderer.cpp>
#include <Input/Input.h>
#include <Time/Time.h>
#include "PlayerCharacter.h"

LPCTSTR g_szClassName = TEXT("윈도우 클래스 이름");
//enum ECharacterName g_eCurrentCharacter = boy;
APlayerCharacter* g_Character = nullptr;
int MoveWeight = 5;

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
		int cols = Renderer::g_width / CELL_SIZE;
		int rows = Renderer::g_height / CELL_SIZE;

		RECT rect = { 0, 0, Renderer::g_width, Renderer::g_height };
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
			g_Character->animstate = APlayerCharacter::AnimationState::Idle;
		}
		if (wParam == VK_2)
		{
			g_Character->animstate = APlayerCharacter::AnimationState::Attack;
		}
		if (wParam == VK_3)
		{
			g_Character->animstate = APlayerCharacter::AnimationState::Run;
		}

		if (Input::IsKeyDown(VK_RIGHT))
		{
			g_Character->dirState = APlayerCharacter::DirState::Right;
			g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip = 0;
			g_Character->SetActorLocation(g_Character->GetActorLocation().x + MoveWeight, g_Character->GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_LEFT))
		{
			g_Character->dirState = APlayerCharacter::DirState::Left;
			g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip = 0;
			g_Character->SetActorLocation(g_Character->GetActorLocation().x - MoveWeight, g_Character->GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_DOWN))
		{
			g_Character->dirState = APlayerCharacter::DirState::Bottom;
			g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip = 0;
			g_Character->SetActorLocation(g_Character->GetActorLocation().x, g_Character->GetActorLocation().y + MoveWeight);
		}
		if (Input::IsKeyDown(VK_UP))
		{
			g_Character->dirState = APlayerCharacter::DirState::Top;
			g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip = 0;
			g_Character->SetActorLocation(g_Character->GetActorLocation().x, g_Character->GetActorLocation().y - MoveWeight);
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

// 메인 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
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
	RECT rcClient = { 0, 0, (LONG)Renderer::g_width, (LONG)Renderer::g_height };
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
	Renderer::Initialize(hwnd);
	//Gdiplus::Bitmap* g_pImageBitmap[MAX_CHARACTER_SIZE][MAX_VIDEO_SIZE];
	g_Character = new APlayerCharacter();
	g_Character->Initialize();
	g_Character->SetActorLocation(50, 50);

	// Time::Initialize
	Time::Initialize();
	float m_fFPSTimeAnimationScene = 1.0f / 24.0f;
	float m_fFPSLastTimeAnimationScene = Time::GetTotalTime();
	float m_fcountOneSecondAnimationScene = Time::GetTotalTime();

	MSG msg;
	while (true)
	{
		Time::UpdateTime();
		Input::Update();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_fFPSLastTimeAnimationScene = Time::GetTotalTime() - m_fcountOneSecondAnimationScene;
		if (m_fFPSLastTimeAnimationScene >= m_fFPSTimeAnimationScene)	// 1/60 초에 한 번씩
		{
			if (g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationMaxSize != 0)
			{
			g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip =
				(g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip + 1)
				% g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationMaxSize;
			}
			m_fcountOneSecondAnimationScene = Time::GetTotalTime();
		}

		// Renderer::BeginDraw
		Renderer::BeginDraw();

		//Renderer::Render;
		if (g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_frames)
		{
			Renderer::RenderImage(
				g_Character
				->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]
				->m_frames[g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip]
				->m_frame,
				g_Character->GetActorLocation().x,
				g_Character->GetActorLocation().y,
				g_Character
				->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]
				->m_frames[g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip]
				->m_frameScale.x,
				g_Character
				->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]
				->m_frames[g_Character->AnimationBundle.animationComponent[(int)g_Character->dirState][(int)g_Character->animstate]->m_ianimationClip]
				->m_frameScale.y
			);

		}

		// Renderer::EndDraw
		Renderer::EndDraw();
	}

	delete g_Character;

	// Renderer::Release
	Renderer::Release(hwnd);
	//////////////////////////////////////////////////////////////////////////

	UninitConsole();  // 콘솔 출력 해제
	return (int)msg.wParam;
}
