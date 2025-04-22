#pragma once
#include <Classes/Character.h>
#include <Math/Math.h>
#include <UI/UIText.h>

class AEnemyCharacter : public ACharacter
{
public:
	AEnemyCharacter();
	~AEnemyCharacter();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Release() override;

	void LoadData(std::wstring baseDir, std::wstring fileName);

private:
	SUIText* m_textui;
	float m_randomWidth;
	float m_randomHeight;
};