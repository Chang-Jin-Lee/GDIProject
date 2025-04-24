#include <Runtime/Core/FIleHelper.h>
#include "BackGroundImage.h"
#include <Runtime/Renderer/Renderer.h>

ABackGroundImage::ABackGroundImage()
{

}

ABackGroundImage::~ABackGroundImage()
{

}

void ABackGroundImage::Initialize()
{
	__super::Initialize();
}

void ABackGroundImage::Update()
{
	__super::Update();
	Renderer::RenderActor(this);
}

void ABackGroundImage::LoadData(std::wstring baseDir, std::wstring fileName)
{
	LoadStaticMeshData(baseDir, fileName);
}
