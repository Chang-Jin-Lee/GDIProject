#pragma once
#include <Classes/Actor.h>
#include "../Character/PlayerCharacter.h"

class ACity : public AActor
{
public:
    int ProductionPoints = 0;
    std::vector<std::shared_ptr<APlayerCharacter>> ProducedUnits;

    virtual void Initialize() override;
    virtual void Update() override;

    std::shared_ptr<APlayerCharacter> ProduceUnit(EUnitType Type);
};
