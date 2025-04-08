#pragma once

#include <Classes/Scene/Scene.h>

class UMenuScene : public UScene
{
public:
	UMenuScene() {}
	~UMenuScene() {}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;
};