#pragma once
#include <Classes/Character.h>
#include <Runtime/Animation/AnimationComponent.h>

enum class EUnitType
{
    Settler,
    Warrior,
    Archer,
};

class AUnit : public ACharacter
{
public:
    AUnit();
    ~AUnit();
    AUnit(EUnitType Type);

    virtual void Initialize() override;
    virtual void Update() override;

    void Attack(AUnit* Target);
    void MoveTo(const FVector2& TargetPosition);

public:
    EUnitType UnitType;
    int Health = 100;
    int AttackDamage = 10;
    int MoveRange = 1;
    int AttackRange = 1;
    bool bIsDead = false;
};
