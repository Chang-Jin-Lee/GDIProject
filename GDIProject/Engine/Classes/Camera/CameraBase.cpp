#include "CameraBase.h"
#include "../Components/SceneComponent.h"

UCamaraBase::UCamaraBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(L"SceneComponent");
}

UCamaraBase::~UCamaraBase()
{
	SceneComponent.reset();
}

void UCamaraBase::Initialize()
{
	__super::Initialize();
}

void UCamaraBase::Update()
{
	__super::Update();
}

void UCamaraBase::Release()
{
	__super::Release();
	SceneComponent.reset();
}

