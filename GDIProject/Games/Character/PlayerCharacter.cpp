#include "PlayerCharacter.h"
#include <Runtime/Core/FIleHelper.h>
#include <Runtime/Renderer/Renderer.h>
#include <Time/Time.h>
#include <iostream>
#include "../Games.h"
#include "../Core/GameConfig.h"
#include <Input/Input.h>
#include "../Scene/PlayScene.h"
#include "../UI/CharacterNameWidget.h"
#include <UI/UITextComponent.h>
#include <Classes/Components/StaticMeshComponent.h>

APlayerCharacter::APlayerCharacter()
{
	bStatic = false;

    for (int j = 0; j < static_cast<int>(AnimationState::Max); j++)
	{
		for (int i = 0; i < static_cast<int>(DirState::Max); i++)
		{
			AnimationBundle.animationComponent[i][j] = new UAnimationComponent();
		}
        AnimationBundle.baseImages[j] = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseImage"));
	}

	dirState = DirState::Bottom;
	animstate = AnimationState::Idle;
	std::wstring Name = TEXT("nameWidget") + std::to_wstring(Time::GetElapsedTime());
	m_nameWidget = CreateDefaultSubobject<UCharacterNameWidget>(Name);
	if(auto m_nameWidgetRef = m_nameWidget.lock())
		m_nameWidgetRef->SetName(Name);
	attachedWidgets.push_back(m_nameWidget);
}

APlayerCharacter::APlayerCharacter(EUnitType Type)
{
	SetUnitType(Type);
}

APlayerCharacter::~APlayerCharacter()
{
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
	//FVector2 Size = FVector2(13.0f, 20.0f);
	//FVector2 Scale = FVector2(1.5f, 1.5f);
	SetActorLocation(Location.x, Location.y);
	auto* anim = AnimationBundle.animationComponent[0][0];
	if (anim && !anim->m_frames.empty() && !anim->m_frames[0].empty() && anim->m_frames[0][0].m_frame)
	{
		Gdiplus::Bitmap* frame = anim->m_frames[0][0].m_frame;
		SetActorSize(frame->GetWidth(), frame->GetHeight());
	}
	//SetActorSize(Size.x, Size.y);
	//SetActorScale(Scale.x, Scale.y);

	// UI 초기화
	if (auto m_nameWidgetRef = m_nameWidget.lock())
	{
		if (auto text = m_nameWidgetRef->m_nameUI.lock())
		{
			text->Initialize(GetName(), 10, (wchar_t*)L"Verdana", Gdiplus::Color(255, 255, 255), FVector2(-60 + GetActorSize().x * GetActorScale().x / 2, -20), FVector2(120.0f, 20.0f));
			text->AttachedUIToActor(weak_from_this());
			text->SetWidgetRenderType(UWidgetComponent::WidgetRenderType::World); // 유닛 머리 위 이름은 월드 공간
		}
	}

	ReadyForNextTurn();
}

void APlayerCharacter::Update()
{
	__super::Update();
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
	m_nameWidget.reset();
}

void APlayerCharacter::ReadyForNextTurn()
{
	ActionRemainCount = ActionMaxCount;
	bSkipTurn = false;
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

//void APlayerCharacter::LoadData(Gdiplus::Bitmap* baseImage, int** cloneInfo, int rowSize, int colSize, DirState dirState, AnimationState animState, int pixelformat)
//{
//	AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(animState)]->LoadData(baseImage, cloneInfo, rowSize, colSize, pixelformat);
//}

void APlayerCharacter::LoadData(Gdiplus::Bitmap* baseImage, std::vector<std::vector<int>>& infos, DirState dirState, AnimationState animState, int pixelformat)
{
	AnimationBundle.animationComponent[static_cast<int>(dirState)][static_cast<int>(animState)]->LoadData(baseImage, infos, pixelformat);
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
	ApplyStats(GameConfig::LoadFromResource().GetUnit(Type));
}

void APlayerCharacter::SetUnitType(int value)
{
	SetUnitType(static_cast<EUnitType>(value));
}

void APlayerCharacter::ApplyStats(const UnitStats& Stats)
{
	UnitType = Stats.Type;
	Health = Stats.Health;
	AttackDamage = Stats.AttackDamage;
	MoveRange = Stats.MoveRange;
	AttackRange = Stats.AttackRange;
	ActionMaxCount = Stats.ActionMaxCount;
	ActionRemainCount = Stats.ActionMaxCount;
	CanMelee = Stats.CanMelee;
	CanRanged = Stats.CanRanged;
	bIsDead = false;
}

std::wstring APlayerCharacter::GetUnitTypeString(int value)
{
	EUnitType Type = static_cast<EUnitType>(value);
	return GameConfig::LoadFromResource().GetUnit(Type).DisplayName;
}
