#pragma once
#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include "../../Math/Math.h"
#pragma comment(lib, "gdiplus.lib")

class UAnimationComponent
{
public:
	struct FFrame
	{
		Gdiplus::Bitmap* m_frame;
		FVector2 m_frameScale;

		FFrame()
		{
			m_frame = nullptr;
			m_frameScale.x = 0;
			m_frameScale.y = 0;
		}
	};

	FFrame** m_frames;
	UAnimationComponent() {}
	UAnimationComponent(int rowSize, int colSize);
	~UAnimationComponent() {}

	void Initialize(int rowSize, int colSize);
	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, int pixelformat);
};