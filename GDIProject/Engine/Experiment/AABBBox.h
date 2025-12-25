#pragma once
template<typename T> class TVector2; using FVector2 = TVector2<float>;

class FAABBBox
{
public:
    FAABBBox();
    FAABBBox(const float& minX, const  float& minY, const float& maxX, const float& maxY);
    ~FAABBBox();

    void SetMinVector(float x, float y);
    void SetMaxVector(float x, float y);

    FVector2 GetMinVector();
    FVector2 GetMaxVector();

    bool IsValidBox();
    FVector2 GetCenter();

private:
    struct Impl;
    Impl* pImpl; // 수명은 cpp에서 관리
};