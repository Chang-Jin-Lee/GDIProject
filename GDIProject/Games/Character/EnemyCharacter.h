#pragma once
#include <Classes/Character.h>
#include <Math/Math.h>

class SUITextComponent;

class AEnemyCharacter : public AActor
{
public:
	AEnemyCharacter();
	~AEnemyCharacter();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void LoadData(std::wstring baseDir, std::wstring fileName);

private:
	std::shared_ptr<SUITextComponent> m_textui;
	float m_randomWidth;
	float m_randomHeight;
};