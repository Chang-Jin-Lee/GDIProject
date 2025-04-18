#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>

class ABackGroundImage : public AActor
{
public:
	ABackGroundImage();
	~ABackGroundImage();

	void Initialize();
	void LoadData(std::wstring baseDir, std::wstring fileName);
};