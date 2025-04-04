#pragma once

template<typename T>
class TVector2
{
public:
	T x, y;

	TVector2() {}
	TVector2(T _x, T _y) : x(_x), y(_y) {}

	TVector2(const TVector2<T>& left) { x = left.x; y = left.y; }
	~TVector2() {}
};

using FVector2 = TVector2<float>;