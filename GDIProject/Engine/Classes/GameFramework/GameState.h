#pragma once
#include <functional>
#include <string>
#include <unordered_map>

class AGameStateBase
{
public:
	AGameStateBase();
	virtual ~AGameStateBase();
	virtual void Initialize() = 0;
	virtual void Release() = 0;

	using CreatorFunc = std::function<AGameStateBase*()>;
	static void RegistGameState(const std::wstring& name, CreatorFunc func);
	static AGameStateBase* CreateInstance();

private:
	static std::unordered_map<std::wstring, CreatorFunc>& Registry();
};