#pragma once

#include <Classes/Scene/Scene.h>
#include "../Image/BackGroundImage.h"

class UEndScene : public UScene
{
public:
	UEndScene();
	~UEndScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	ABackGroundImage* m_image;
};