#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>

class SUITextComponent;

class APlayerCharacter : public ACharacter
{
public:
	APlayerCharacter();
	~APlayerCharacter();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void Input();

	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat);
	void SetAnimMeshScale(float width, float height);
	
	bool m_bAttackAnimationPlaying = false;
	SUITextComponent* m_textui;

	const int RotateSpeed = 200;
	const float MoveSpeed = 600;
	float m_fcharacterRotationSpeed = 85;
};