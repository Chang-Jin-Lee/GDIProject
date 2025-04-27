#include "GameState.h"

GameStateBase::GameStateBase()
{
	m_gMainCamera = std::make_shared<ACameraActor>();
}

GameStateBase::~GameStateBase()
{
}

void GameStateBase::RegistGameState(const std::wstring& name, CreatorFunc func)
{
	Registry()[name] = func;
}

GameStateBase* GameStateBase::CreateInstance()
{
	if (Registry().empty()) return nullptr; // 없으면 생성 안 함
	return Registry().begin()->second();    // 첫 번째 등록된 클래스 생성
}

std::unordered_map<std::wstring, GameStateBase::CreatorFunc>& GameStateBase::Registry()
{
	static std::unordered_map<std::wstring, CreatorFunc> registry;
	return registry;
}
