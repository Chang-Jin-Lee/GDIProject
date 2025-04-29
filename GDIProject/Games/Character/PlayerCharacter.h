#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>

enum class EUnitType
{
	Settler,
	Warrior,
	Archer,
};

class SUITextComponent;
class UCharacterNameWidget;

class APlayerCharacter : public ACharacter
{
public:
	APlayerCharacter();
	APlayerCharacter(EUnitType Type);
	~APlayerCharacter();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void Input();

	void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat);
	void SetAnimMeshScale(float width, float height);
	
	bool m_bAttackAnimationPlaying = false;
	std::shared_ptr<UCharacterNameWidget> m_nameWidget;

public:
	const int RotateSpeed = 200;
	const float MoveSpeed = 600;
	float m_fcharacterRotationSpeed = 85;

	// Turn 게임 관련 
	void Attack(APlayerCharacter* Target);
	void MoveTo(const FVector2& TargetPosition);

	void SetUnitType(EUnitType Type);
	void SetUnitType(int value);
	static std::wstring GetUnitTypeString(int value);
public:
	EUnitType UnitType = EUnitType::Settler;
	int Health = 100;
	int AttackDamage = 10;
	int MoveRange = 1;
	int AttackRange = 1;
	bool bIsDead = false;
	int ActionCount = 0;
};