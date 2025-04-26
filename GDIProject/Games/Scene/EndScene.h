#pragma once

#include <Classes/Scene/Scene.h>

class ABackGroundImage;
class UEndScene_ScoreGuide;

class UEndScene : public UScene
{
public:
	UEndScene();
	~UEndScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	virtual void DeleteNullObjects() override;

	void UIInitialize();
	void UpdateInput();

	std::shared_ptr<UEndScene_ScoreGuide> m_scoreGuide;
	std::shared_ptr<ABackGroundImage> m_image;
};