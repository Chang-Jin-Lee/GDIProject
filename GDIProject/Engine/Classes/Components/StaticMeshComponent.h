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
	FVector2 m_fmeshScale;
	Gdiplus::Bitmap* mesh;
	
	UStaticMeshComponent();
	UStaticMeshComponent(std::wstring baseDir, std::wstring fileName);
	~UStaticMeshComponent();

	void LoadData(std::wstring baseDir, std::wstring fileName);
	void Update();

	void SetMeshScale(float width, float height)
	{
		m_fmeshScale.x = width;
		m_fmeshScale.y = height;
	}

	FVector2 GetMeshScale() { return m_fmeshScale; }
};