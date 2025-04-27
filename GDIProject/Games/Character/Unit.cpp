#include "Unit.h"

AUnit::AUnit()
{
}

AUnit::~AUnit()
{
}

AUnit::AUnit(EUnitType Type)    // 직렬화 필요 
{
    UnitType = Type;

    if (Type == EUnitType::Settler)
    {
        Health = 50;
        AttackDamage = 0;
        MoveRange = 2;
    }
    else if (Type == EUnitType::Warrior)
    {
        Health = 100;
        AttackDamage = 20;
        MoveRange = 1;
    }
    else if (Type == EUnitType::Archer)
    {
        Health = 70;
        AttackDamage = 15;
        MoveRange = 1;
        AttackRange = 2;
    }
}

void AUnit::Initialize() 
{
    __super::Initialize();
}

void AUnit::Update() 
{
    __super::Update();
}

void AUnit::Attack(AUnit* Target)
{
    if (Target)
    {
        Target->Health -= AttackDamage;
        if (Target->Health <= 0)
        {
            Target->bIsDead = true;
        }
    }
}

void AUnit::MoveTo(const FVector2& TargetPosition)
{
    SetActorLocation(TargetPosition);
}
