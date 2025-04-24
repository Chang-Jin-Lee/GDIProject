#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>

class ABackGroundImage : public AActor
{
public:
	ABackGroundImage();
	~ABackGroundImage();

	virtual void Initialize() override;
	virtual void Update() override;
	void LoadData(std::wstring baseDir, std::wstring fileName);
};