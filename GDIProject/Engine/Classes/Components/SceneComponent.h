#pragma once


class USceneComponent
{
public:
	int x, y;
	
	USceneComponent() : x(0), y(0) {};
	USceneComponent(int _x, int _y) : x(_x), y(_y) {};
	~USceneComponent() {};
};