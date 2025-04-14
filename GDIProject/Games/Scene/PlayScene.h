#pragma once

#include <Classes/Scene/Scene.h>
#include "../PlayerCharacter.h"
#include <Time/Time.h>

class UPlayScene : public UScene
{
public:
	UPlayScene();
	~UPlayScene();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;
	virtual void Release() override;

	void Input();

private:
	APlayerCharacter* m_fPlayerCharacter;
};