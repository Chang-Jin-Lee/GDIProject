#pragma once
#include <wchar.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

class UObjectBase
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
};