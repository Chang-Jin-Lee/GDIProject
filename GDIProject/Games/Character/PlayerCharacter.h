#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>
#include "../Core/GameTypes.h"
#include <string>
#include <vector>


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

	void ReadyForNextTurn();

	void Input();

	//void LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat);
	void LoadData(Gdiplus::Bitmap* baseImage, std::vector<std::vector<int>>& infos, DirState dirState, AnimationState animState, int pixelformat);
	void SetAnimMeshScale(float width, float height);
	
	bool m_bAttackAnimationPlaying = false;
	std::weak_ptr<UCharacterNameWidget> m_nameWidget;

public:
	const int RotateSpeed = 200;
	const float MoveSpeed = 600;
	float m_fcharacterRotationSpeed = 85;

	// Turn-game helpers
	void Attack(APlayerCharacter* Target);
	void MoveTo(const FVector2& TargetPosition);

	void SetUnitType(EUnitType Type);
	void SetUnitType(int value);
	void ApplyStats(const UnitStats& Stats);
	static std::wstring GetUnitTypeString(int value);
public:
	EUnitType UnitType = EUnitType::Settler;
	std::string StableId;
	EPlayerSlot OwnerSlot = EPlayerSlot::None;
	int Health = 100;
	int AttackDamage = 10;
	int MoveRange = 1;
	int AttackRange = 1;
	bool CanMelee = false;
	bool CanRanged = false;
	bool bIsDead = false;
	int ActionMaxCount = 0;
	int ActionRemainCount = 0;
	bool bSkipTurn = false;

	// Tile coordinates in the turn game
	int row = 0;
	int col = 0;
};
