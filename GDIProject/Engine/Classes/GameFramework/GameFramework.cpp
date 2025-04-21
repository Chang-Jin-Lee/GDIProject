#include "GameState.h"

AGameStateBase::AGameStateBase()
{
}

AGameStateBase::~AGameStateBase()
{
}

void AGameStateBase::RegistGameState(const std::wstring& name, CreatorFunc func)
{
	Registry()[name] = func;
}

AGameStateBase* AGameStateBase::CreateInstance()
{
	if (Registry().empty()) return nullptr; // 없으면 생성 안 함
	return Registry().begin()->second();    // 첫 번째 등록된 클래스 생성
}

std::unordered_map<std::wstring, AGameStateBase::CreatorFunc>& AGameStateBase::Registry()
{
	static std::unordered_map<std::wstring, CreatorFunc> registry;
	return registry;
}
