#include "AnimationComponent.h"
#include "../Renderer/Renderer.h"


UAnimationComponent::UAnimationComponent(int rowSize, int colSize)
{
	Initialize(rowSize, colSize);
}

UAnimationComponent::~UAnimationComponent()
{
	Release();
}

void UAnimationComponent::Initialize(int rowSize, int colSize)
{
	Release(); // 기존 데이터 해제
	m_ianimationMaxSize = rowSize;

	m_frames.resize(rowSize);
	for (int i = 0; i < rowSize; ++i)
	{
		m_frames[i].resize(colSize);
	}
}

void UAnimationComponent::LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, int pixelformat)
{
	if (!cloneInfo || m_frames.empty()) return;

	for (int i = 0; i < rowSize && i < m_frames.size(); ++i)
	{
		if (!cloneInfo[i] || colSize <= 3) continue;

		int left = cloneInfo[i][0], top = cloneInfo[i][1], right = cloneInfo[i][2], bottom = cloneInfo[i][3];
		int width = right - left;
		int height = bottom - top;

		FFrame& frame = m_frames[i][0];

		if (frame.m_frame)
		{
			delete frame.m_frame;
			frame.m_frame = nullptr;
		}

		frame.m_frameSize = FVector2((float)width, (float)height);
		frame.m_frame = baseImage->Clone(left, top, width, height, pixelformat);
	}
}

void UAnimationComponent::Release()
{
	for (auto& row : m_frames)
	{
		for (auto& frame : row)
		{
			if (Renderer::IsGdiValid() && frame.m_frame)
			{
				delete frame.m_frame;
				frame.m_frame = nullptr;
			}
		}
	}
	m_frames.clear();
	m_ianimationMaxSize = 0;
	m_ianimationClip = 0;
}
