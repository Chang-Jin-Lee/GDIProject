#pragma once
#include <Classes/Actor.h>
#include <Runtime/Animation/AnimationComponent.h>

enum class ETileType
{
	Desert,
	Grassland,
	Hills,
	Plain,
	Mountain
};

class ATile : public AActor
{
public:
	ATile();
	~ATile();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LoadData() override;

	ETileType m_etileType = ETileType::Desert;
};