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

	FFrame** m_frames;						// 애니메이션의 한 장

	/* 애니메이션 재생 관련 */
	unsigned int m_ianimationClip = 0;		// 현재 애니메이션의 몇 초 부분에 있는지
	unsigned int m_ianimationMaxSize = 0;	// 애니메이션의 최대 크기. ex) 몇 초짜리 애니메이션인지 등

	unsigned int m_irowSize = 0;			// FFrame** delete를 위해 필요한 사이즈
	unsigned int m_icolSize = 0;
	UAnimationComponent() : m_ianimationClip(0), m_ianimationMaxSize(0) {}
	UAnimationComponent(int rowSize, int colSize);
	~UAnimationComponent();

	void Initialize(int rowSize, int colSize);
	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, int pixelformat);
};