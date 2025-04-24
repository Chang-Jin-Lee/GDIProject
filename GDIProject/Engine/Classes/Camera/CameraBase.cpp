#include "CameraBase.h"

UCamaraBase::UCamaraBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(L"SceneComponent");
}

UCamaraBase::~UCamaraBase()
{
	delete SceneComponent;
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
}

