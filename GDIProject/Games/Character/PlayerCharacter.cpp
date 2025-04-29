#include "PlayerCharacter.h"
#include <Runtime/Core/FIleHelper.h>
#include <Runtime/Renderer/Renderer.h>
#include <Time/Time.h>
#include <iostream>
#include "../Games.h"
#include <Input/Input.h>
#include "../Scene/PlayScene.h"
#include "../UI/CharacterNameWidget.h"
#include <UI/UITextComponent.h>

APlayerCharacter::APlayerCharacter()
{
	bStatic = false;

	for (int j = 0; j < static_cast<int>(AnimationState::Max); j++)
	{
		for (int i = 0; i < static_cast<int>(DirState::Max); i++)
		{
			AnimationBundle.animationComponent[i][j] = new UAnimationComponent();
		}
		AnimationBundle.baseImages[j] = new UStaticMeshComponent();
	}

	dirState = DirState::Bottom;
	animstate = AnimationState::Idle;
	std::wstring Name = TEXT("nameWidget") + std::to_wstring(Time::GetElapsedTime());
	m_nameWidget = CreateDefaultSubobject<UCharacterNameWidget>(Name);
	m_nameWidget->SetName(Name);
	attachedWidgets.push_back(m_nameWidget);
}

APlayerCharacter::APlayerCharacter(EUnitType Type)
{
	SetUnitType(Type);
}

APlayerCharacter::~APlayerCharacter()
{
	for (int i = 0; i < static_cast<int>(DirState::Max); i++)
	{
		for (int j = 0; j < static_cast<int>(AnimationState::Max); j++)
		{
			delete AnimationBundle.animationComponent[i][j];
		}
		//delete AnimationBundle.baseImages[i];
	}
	m_nameWidget.reset();
}

void APlayerCharacter::Initialize()
{
	__super::Initialize();

	for (int dirState = 0; dirState < static_cast<int>(DirState::Max); dirState++)
	{
		LoadAnimationData(L"Character1/Unarmed_Idle", L"Unarmed", L',', AnimationState::Idle, static_cast<DirState>(dirState));
		LoadAnimationData(L"Character1/Unarmed_Run", L"Unarmed", L',', AnimationState::Run, static_cast<DirState>(dirState));
		LoadAnimationData(L"Character1/Sword_Attack", L"Sword", L',', AnimationState::Attack, static_cast<DirState>(dirState));
	}

	m_fcharacterRotationSpeed = 360;
	bPlayingAnimation = true;

	// SceneComponent 값 초기화
	FVector2 Location = FVector2(Renderer::GetResolution().x * 0.5f, Renderer::GetResolution().y * 0.5f);
	FVector2 Size = FVector2(13.0f, 20.0f);
	FVector2 Scale = FVector2(1.5f, 1.5f);
	SetActorLocation(Location.x, Location.y);
	SetActorSize(Size.x, Size.y);
	SetActorScale(Scale.x, Scale.y);

	// UI 초기화
	m_nameWidget->m_nameUI->Initialize(GetName(), 10, (wchar_t*)L"Verdana", Gdiplus::Color(255, 255, 255), FVector2(-60 + GetActorSize().x * GetActorScale().x / 2, -20), FVector2(120.0f, 20.0f));
	m_nameWidget->m_nameUI->AttachedUIToActor(this);
}

void APlayerCharacter::Update()
{
	__super::Update();
	//FVector2 mouseclick = Game::GetLMouseClickPosition();
	//if (mouseclick.IsZero() == false)
	//{
	//	FVector2 dir = (mouseclick - GetActorLocation()).Normalize();
	//	if ((mouseclick - GetActorLocation()).Length() > 0.1f)
	//	{
	//		FVector2 location = GetActorLocation();
	//		FVector2 updateLocation = location + dir * MoveSpeed * Time::GetElapsedTime();
	//		SetActorLocation(updateLocation.x, updateLocation.y);
	//	}
	//	else
	//	{
	//		Game::SetLMouseClickPosition(FVector2(0, 0));
	//	}
	//}

	if (AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip == AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationMaxSize - 1)
	{
		if (animstate == ACharacter::AnimationState::Attack)
		{
			m_bAttackAnimationPlaying = false;
		}
	}

	// 카메라 부착
	//Game::GetGameState()->GetMainCamera().get()->SetCameraLocation(GetActorLocation() - (Renderer::GetResolution() / 2));

	Input();
}

void APlayerCharacter::Release()
{
	__super::Release();
}

void APlayerCharacter::Input()
{

	if (Input::IsKeyDown(VK_R))
	{
		float rotation = GetActorRotation();
		SetActorRotation(rotation + Time::GetElapsedTime() * m_fcharacterRotationSpeed);
	}

	if (Input::IsKeyDown(VK_SPACE))
	{
		if (m_bAttackAnimationPlaying == false)
		{
			AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip = 0;
			m_bAttackAnimationPlaying = true;
			animstate = APlayerCharacter::AnimationState::Attack;
		}
	}

	if (Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_UP) || Input::IsKeyDown(VK_DOWN))
	{
		if (m_bAttackAnimationPlaying == false)
			animstate = APlayerCharacter::AnimationState::Run;
		if (Input::IsKeyDown(VK_RIGHT))
		{
			dirState = APlayerCharacter::DirState::Right;
			//AnimationBundle.animationComponent[(int)dirState][(int)animstate]->m_ianimationClip = 0;
			SetActorLocation(GetActorLocation().x + RotateSpeed * Time::GetElapsedTime(), GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_LEFT))
		{
			dirState = APlayerCharacter::DirState::Left;
			SetActorLocation(GetActorLocation().x - RotateSpeed * Time::GetElapsedTime(), GetActorLocation().y);
		}
		if (Input::IsKeyDown(VK_DOWN))
		{
			dirState = APlayerCharacter::DirState::Bottom;
			SetActorLocation(GetActorLocation().x, GetActorLocation().y + RotateSpeed * Time::GetElapsedTime());
		}
		if (Input::IsKeyDown(VK_UP))
		{
			dirState = APlayerCharacter::DirState::Top;
			SetActorLocation(GetActorLocation().x, GetActorLocation().y - RotateSpeed * Time::GetElapsedTime());
		}
	}
	else
	{
		if (m_bAttackAnimationPlaying == false)
			animstate = APlayerCharacter::AnimationState::Idle;
	}
}

void APlayerCharacter::LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat)
{
	AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(animState)]->LoadData(baseImage, cloneInfo, rowSize, colSize, pixelformat);
}

void APlayerCharacter::SetAnimMeshScale(float width, float height)
{

}

void APlayerCharacter::Attack(APlayerCharacter* Target)
{
	if (Target)
	{
		Target->Health -= AttackDamage;
		if (Target->Health <= 0)
		{
			Target->bIsDead = true;
		}
	}
}

void APlayerCharacter::MoveTo(const FVector2& TargetPosition)
{
	SetActorLocation(TargetPosition);
}

void APlayerCharacter::SetUnitType(EUnitType Type)
{
	UnitType = Type;

	switch (Type)
	{
	case EUnitType::Settler:
		Health = 50;
		AttackDamage = 0;
		MoveRange = 2;
		ActionCount = 1;
		break;
	case EUnitType::Warrior:
		Health = 100;
		AttackDamage = 20;
		MoveRange = 1;
		ActionCount = 3;
		break;
	case EUnitType::Archer:
		Health = 70;
		AttackDamage = 15;
		MoveRange = 1;
		AttackRange = 2;
		ActionCount = 4;
		break;
	default:
		break;
	}
}

void APlayerCharacter::SetUnitType(int value)
{
	EUnitType Type = static_cast<EUnitType>(value);
	UnitType = Type;

	switch (Type)
	{
	case EUnitType::Settler:
		Health = 50;
		AttackDamage = 0;
		MoveRange = 2;
		ActionCount = 1;
		break;
	case EUnitType::Warrior:
		Health = 100;
		AttackDamage = 20;
		MoveRange = 1;
		ActionCount = 3;
		break;
	case EUnitType::Archer:
		Health = 70;
		AttackDamage = 15;
		MoveRange = 1;
		AttackRange = 2;
		ActionCount = 4;
		break;
	default:
		break;
	}
}

std::wstring APlayerCharacter::GetUnitTypeString(int value)
{
	EUnitType Type = static_cast<EUnitType>(value);

	switch (Type)
	{
	case EUnitType::Settler:
		return L"개척자";
	case EUnitType::Warrior:
		return L"전사";
	case EUnitType::Archer:
		return L"궁수";
	default:
		return L"default";
	}
}
