#pragma once

#include <Classes/Scene/Scene.h>

class ABackGroundImage;
class UEndScene_Widget;

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

	// delegate function
	void StartGame();

	std::weak_ptr<UEndScene_Widget> m_Widget;
	std::weak_ptr<ABackGroundImage> m_image;
};