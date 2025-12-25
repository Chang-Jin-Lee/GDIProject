#pragma once
class USceneComponent; // forward decl
#include "../Actor.h"

class UCamaraBase : public AActor
{
public:
	UCamaraBase();
	~UCamaraBase();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
};