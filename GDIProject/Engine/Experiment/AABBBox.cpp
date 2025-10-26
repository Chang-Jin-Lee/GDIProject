#include "AABBBox.h"
#include "../Math/Math.h"

struct FAABBBox::Impl
{
    FVector2 minVector{0, 0};
    FVector2 maxVector{0, 0};
};

FAABBBox::FAABBBox()
    : pImpl(new Impl())
{
}

FAABBBox::FAABBBox(const float& minX, const  float& minY, const float& maxX, const float& maxY)
    : pImpl(new Impl())
{
    pImpl->minVector = FVector2(minX, minY);
    pImpl->maxVector = FVector2(maxX, maxY);
}

FAABBBox::~FAABBBox()
{
    delete pImpl;
    pImpl = nullptr;
}

void FAABBBox::SetMinVector(float x, float y)
{
    pImpl->minVector = FVector2(x, y);
}

void FAABBBox::SetMaxVector(float x, float y)
{
    pImpl->maxVector = FVector2(x, y);
}

FVector2 FAABBBox::GetMinVector()
{
    return pImpl->minVector;
}

FVector2 FAABBBox::GetMaxVector()
{
    return pImpl->maxVector;
}

bool FAABBBox::IsValidBox()
{
    return pImpl->minVector != FVector2(-1, -1) && pImpl->maxVector != FVector2(-1, -1);
}

FVector2 FAABBBox::GetCenter()
{
    return FVector2((pImpl->minVector + pImpl->maxVector) * 0.5f);
}


