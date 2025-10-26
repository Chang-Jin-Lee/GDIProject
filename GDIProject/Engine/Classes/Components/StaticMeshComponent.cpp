#include "StaticMeshComponent.h"
#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include "../../Math/Math.h"
#include "../../Runtime/Renderer/Renderer.h"
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

struct UStaticMeshComponent::Impl
{
	FVector2 meshSize{-1, -1};
	Gdiplus::Bitmap* mesh{nullptr};
};

UStaticMeshComponent::UStaticMeshComponent()
{
	pImpl = std::make_unique<Impl>();
}

UStaticMeshComponent::UStaticMeshComponent(std::wstring baseDir, std::wstring fileName)
{
	pImpl = std::make_unique<Impl>();
	wchar_t wcsbuf[100];
	int     num;
	num = swprintf(wcsbuf, 100, L"../Resource/%s/%s", baseDir.c_str(), fileName.c_str());
	pImpl->mesh = new Gdiplus::Bitmap(wcsbuf);
	pImpl->meshSize = FVector2((float)pImpl->mesh->GetWidth(), (float)pImpl->mesh->GetHeight());
}

UStaticMeshComponent::~UStaticMeshComponent()
{
	if (Renderer::IsGdiValid() && pImpl && pImpl->mesh)
	{
		delete pImpl->mesh;
		pImpl->mesh = nullptr;
	}
}

void UStaticMeshComponent::LoadData(std::wstring baseDir, std::wstring fileName)
{
	wchar_t wcsbuf[100];
	int     num;
	num = swprintf(wcsbuf, 100, L"../Resource/%s/%s", baseDir.c_str(), fileName.c_str());

	pImpl->mesh = new Gdiplus::Bitmap(wcsbuf);
	SetMeshSize((float)pImpl->mesh->GetWidth(), (float)pImpl->mesh->GetHeight());
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
	if (Renderer::IsGdiValid() && pImpl && pImpl->mesh)
	{
		delete pImpl->mesh;
		pImpl->mesh = nullptr;
	}
}

void UStaticMeshComponent::SetMeshSize(float width, float height)
{
	pImpl->meshSize = FVector2(width, height);
}

void UStaticMeshComponent::SetMeshScale(float widthRatio, float heightRatio)
{
	pImpl->meshSize *= FVector2(widthRatio, heightRatio);
}

FVector2 UStaticMeshComponent::GetMeshSize() const
{
	return pImpl->meshSize;
}

Gdiplus::Bitmap* UStaticMeshComponent::GetMesh() const
{
	return pImpl->mesh;
}

void UStaticMeshComponent::SetMesh(Gdiplus::Bitmap* bitmap)
{
	pImpl->mesh = bitmap;
}
