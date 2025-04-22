#pragma once
#include "../Actor.h"
#include "../../Classes/Components/StaticMeshComponent.h"

class UCamaraBase : public UStaticMeshComponent
{
public:
	UCamaraBase();
	~UCamaraBase();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	USceneComponent* GetSceneComponent() { return SceneComponent; }

private:
	USceneComponent* SceneComponent;
};