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
		 
	mesh = new Gdiplus::Bitmap(wcsbuf);
}

void UStaticMeshComponent::Update()
{

}
