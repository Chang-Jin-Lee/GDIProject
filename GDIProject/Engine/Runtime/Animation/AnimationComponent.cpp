#include "AnimationComponent.h"


UAnimationComponent::UAnimationComponent(int rowSize, int colSize)
{
	m_frames = (FFrame**)malloc(sizeof(FFrame*) * rowSize);
	for (int i = 0; i < rowSize; i++)
	{
		m_frames[i] = (FFrame*)malloc(sizeof(FFrame) * colSize);
	}
}

void UAnimationComponent::Initialize(int rowSize, int colSize)
{
	m_frames = (FFrame**)malloc(sizeof(FFrame*) * rowSize);
	for (int i = 0; i < rowSize; i++)
	{
		m_frames[i] = (FFrame*)malloc(sizeof(FFrame) * colSize);
	}
}

void UAnimationComponent::LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, int pixelformat)
{
	//mesh = baseBitMap;
	if (cloneInfo)
	{
		for (int i = 0; i < rowSize; i++)
		{
			if (cloneInfo[i] && colSize > 3)
			{
				int left = cloneInfo[i][0], top = cloneInfo[i][1], right = cloneInfo[i][2], bottom = cloneInfo[i][3];
				int width = right - left;
				int height = bottom - top;

				m_frames[i]->m_frameScale.x = width;
				m_frames[i]->m_frameScale.y = height;

				//m_frames[i]->m_frame = baseImage->Clone(left, top, width, height, PixelFormat32bppARGB);
				m_frames[i]->m_frame = baseImage->Clone(left, top, width, height, pixelformat);
			}
		}
	}
	
}
