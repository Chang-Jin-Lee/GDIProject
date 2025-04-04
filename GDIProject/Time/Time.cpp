// Time.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"

// TODO: 라이브러리 함수의 예제입니다.
#pragma once
#include "Time.h"

LARGE_INTEGER frequency;
LARGE_INTEGER prevCounter;
LARGE_INTEGER currentCounter;
LARGE_INTEGER initCounter;
float deltaTime = 0;

namespace Time
{
	void Initialize()
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&initCounter);
	}
	void UpdateTime()
	{
		QueryPerformanceCounter(&currentCounter);
		deltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
		prevCounter = currentCounter;
	}
	float GetElapsedTime()
	{
		return deltaTime;
	}
	float GetTotalTime()
	{
		float totalTime = static_cast<float>(currentCounter.QuadPart - initCounter.QuadPart) / frequency.QuadPart;
		return totalTime;
	}
}
