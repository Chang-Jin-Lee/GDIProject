#include "StaticMeshComponent.h"

UStaticMeshComponent::UStaticMeshComponent()
{
}

UStaticMeshComponent::UStaticMeshComponent(std::wstring baseDir, std::wstring fileName)
{
	wchar_t wcsbuf[100];
	int     num;
	//num = swprintf(wcsbuf, sizeof(wcsbuf), L"../Resource/frames/frame_%04d.png", i + 1);
	num = swprintf(wcsbuf, sizeof(wcsbuf), L"../Resource/%s/%s", baseDir, fileName);
	mesh = new Gdiplus::Bitmap(wcsbuf);
}

UStaticMeshComponent::~UStaticMeshComponent()
{
}

void UStaticMeshComponent::LoadData(std::wstring baseDir, std::wstring fileName)
{
	wchar_t wcsbuf[100];
	int     num;
	//num = swprintf(wcsbuf, sizeof(wcsbuf), L"../Resource/frames/frame_%04d.png", i + 1);
	num = swprintf(wcsbuf, 100, L"../Resource/%s/%s", baseDir.c_str(), fileName.c_str());
		 
	Gdiplus::Bitmap* baseBitMap = new Gdiplus::Bitmap(wcsbuf);

	//mesh = baseBitMap;
	int left = 24, top = 22, right = 38, bottom = 46;
	int width = right - left;
	int height = bottom - top;

	m_fmeshScale.x = width * 1.5f;
	m_fmeshScale.y = height * 1.5f;

	mesh = baseBitMap->Clone(left, top, width, height, PixelFormat32bppARGB);
	//mesh = baseBitMap->Clone(24, 22, 38, 46, PixelFormatAlpha);
}

void UStaticMeshComponent::Update()
{

}
