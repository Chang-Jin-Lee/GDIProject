#pragma once
#include <string>
#include <windows.h>
#include <stdio.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")


using namespace Gdiplus;

class UStaticMeshComponent
{
public:
	UStaticMeshComponent();
	UStaticMeshComponent(std::wstring baseDir, std::wstring fileName);
	~UStaticMeshComponent();

	void LoadData(std::wstring baseDir, std::wstring fileName);

	Gdiplus::Bitmap* mesh;
};