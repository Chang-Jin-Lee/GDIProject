#include <Runtime/Core/FIleHelper.h>
#include "BackGroundImage.h"

ABackGroundImage::ABackGroundImage()
{

}

ABackGroundImage::~ABackGroundImage()
{

}

void ABackGroundImage::Initialize()
{
	
}

void ABackGroundImage::LoadData(std::wstring baseDir, std::wstring fileName)
{
	LoadStaticMeshData(baseDir, fileName);
}
