#pragma once
#include <string>
#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include "../../Math/Math.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

class UStaticMeshComponent
{
public:
	FVector2 m_fmeshSize;
	Gdiplus::Bitmap* mesh;
	
	UStaticMeshComponent();
	UStaticMeshComponent(std::wstring baseDir, std::wstring fileName);
	~UStaticMeshComponent();

	void LoadData(std::wstring baseDir, std::wstring fileName);
	void Update();

	void SetMeshSize(float width, float height)
	{
		m_fmeshSize.x = width;
		m_fmeshSize.y = height;
	}

	void SetMeshScale(float widthRatio, float heightRatio)
	{
		m_fmeshSize.x *= widthRatio;
		m_fmeshSize.y *= heightRatio;
	}

	FVector2 GetMeshSize() { return m_fmeshSize; }
};