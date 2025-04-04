#include "AActor.h"

AActor::AActor()
{
	SceneComponent = new USceneComponent();
	StaticMeshComponent = new UStaticMeshComponent();
}

AActor::~AActor()
{

}

void AActor::LoadData(std::wstring baseDir, std::wstring fileName)
{
	StaticMeshComponent;
}
