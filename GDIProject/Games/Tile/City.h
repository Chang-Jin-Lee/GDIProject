#pragma once
#include <Classes/Actor.h>
#include "../Character/Unit.h"

class ACity : public AActor
{
public:
    int ProductionPoints = 0;
    std::vector<std::shared_ptr<AUnit>> ProducedUnits;

    virtual void Initialize() override;
    virtual void Update() override;

    std::shared_ptr<AUnit> ProduceUnit(EUnitType Type);
};
