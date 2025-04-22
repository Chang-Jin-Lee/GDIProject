#pragma once
#include <string>
#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#include "../../Math/Math.h"
#include "../Object.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// staticMeshComponent는 LoadData로 에셋을 로드해주어야 함.
class UStaticMeshComponent : public UObject
{
public:
	FVector2 m_fmeshSize;
	Gdiplus::Bitmap* mesh;
	
	UStaticMeshComponent();
	UStaticMeshComponent(std::wstring baseDir, std::wstring fileName);
	~UStaticMeshComponent();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void LoadData(std::wstring baseDir, std::wstring fileName);

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
	Gdiplus::Bitmap* GetMesh() { return mesh; }
};