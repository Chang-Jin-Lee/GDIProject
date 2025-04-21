#pragma once

#include <Classes/Scene/Scene.h>
#include "../Image/BackGroundImage.h"
#include <UI/UIText.h>

class UEndScene : public UScene
{
public:
	UEndScene();
	~UEndScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	void UIInitialize();
	void UpdateInput();

	SUIText* m_scoreui;
	SUIText* m_scoreGuideui;

	ABackGroundImage* m_image;
};