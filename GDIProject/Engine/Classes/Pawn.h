#pragma once

#include "Actor.h"

// 물리 충돌을 가지는 Actor를 Pawn으로 정의
class APawn : public AActor
{
public:
	APawn();
	~APawn();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
};