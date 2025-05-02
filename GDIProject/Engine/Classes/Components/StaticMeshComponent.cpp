#include "StaticMeshComponent.h"
#include <iostream>
#include "../../Runtime/Renderer/Renderer.h"

UStaticMeshComponent::UStaticMeshComponent()
{
	m_fmeshSize = FVector2(-1, -1);
	mesh = nullptr;
}

UStaticMeshComponent::UStaticMeshComponent(std::wstring baseDir, std::wstring fileName)
{
	if (Renderer::IsGdiValid() && mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
	wchar_t wcsbuf[100];
	int     num;
	num = swprintf(wcsbuf, 100, L"../Resource/%s/%s", baseDir.c_str(), fileName.c_str());
	mesh = new Gdiplus::Bitmap(wcsbuf);
}

UStaticMeshComponent::~UStaticMeshComponent()
{
	if (Renderer::IsGdiValid() && mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}

void UStaticMeshComponent::LoadData(std::wstring baseDir, std::wstring fileName)
{
	wchar_t wcsbuf[100];
	int     num;
	num = swprintf(wcsbuf, 100, L"../Resource/%s/%s", baseDir.c_str(), fileName.c_str());

	if (Renderer::IsGdiValid() && mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
		 
	mesh = new Gdiplus::Bitmap(wcsbuf);
	SetMeshSize((float)mesh->GetWidth(), (float)mesh->GetHeight());
}

void UStaticMeshComponent::Initialize()
{
	__super::Initialize();
}

void UStaticMeshComponent::Update()
{
	__super::Update();
}

void UStaticMeshComponent::Release()
{
	__super::Release();
	if (Renderer::IsGdiValid() && mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}
