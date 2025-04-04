#pragma once

#include "Pawn.h"

// 애니메이션을 가지는 Pawn을 Character로 정의
class ACharacter : public APawn
{
public:
	ACharacter() {}
	~ACharacter() {}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;
};