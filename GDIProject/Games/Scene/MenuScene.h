#pragma once

#include <Classes/Scene/Scene.h>
#include <Classes/Components/StaticMeshComponent.h>
#include "../BackGroundImage.h"

class UMenuScene : public UScene
{
public:
	UMenuScene();
	~UMenuScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	ABackGroundImage* m_image;
};