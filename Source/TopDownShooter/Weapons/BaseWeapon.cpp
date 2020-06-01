// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "./Characters/BaseCharacter.h"
#include "./Characters/HeroCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;

	/*WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionComp->AttachTo(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->AttachTo(RootComponent);*/



	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	//ProjectileSpawnPoint->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ProjectileSpawnPoint->SetupAttachment(WeaponMesh);

	trigger_pulled = false;
	ability_to_shoot = true;
}


// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Fire()
{
	check(0 && "You must override this");
}

void ABaseWeapon::PullTrigger()
{
	if (!trigger_pulled && ability_to_shoot)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PullTrigger"));
		trigger_pulled = true;

		Fire();
		GetWorld()->GetTimerManager().SetTimer(DelayBetweenShooting, this, &ABaseWeapon::Fire, WeaponConfig.TimeBetweenShots, true);
	}
}


void ABaseWeapon::ReleaseTrigger()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayBetweenShooting);
	if (trigger_pulled)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ReleaseTrigger"));
		ability_to_shoot = false;
		GetWorld()->GetTimerManager().SetTimer(DelayAfterShooting, this, &ABaseWeapon::RecoveryAbilityToShoot, WeaponConfig.TimeBetweenShots, false);
	}
	trigger_pulled = false;
}

void ABaseWeapon::RecoveryAbilityToShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayAfterShooting);
	ability_to_shoot = true;
	//UE_LOG(LogTemp, Warning, TEXT("RecoveryAbilityToShoot"));
}


void ABaseWeapon::AttachToPlayer(ABaseCharacter* NewCharacter)
{
	
	if (Character) {
		Character->Weapon = nullptr;
	}
	Character = NewCharacter;
	Character->Weapon = this;
	AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(Character);
	if (HeroCharacter) {
		HeroCharacter->Weapons.Add(this);
	}
	this->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("GunSocket")));
	
	
}

ABaseWeapon* ABaseWeapon::DetachFromPlayer()
{
	
	if (Character) {
		ReleaseTrigger();
		this->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(Character);
		if (HeroCharacter) {
			HeroCharacter->Weapons.Remove(this);
		}
		Character->Weapon = nullptr;
		Character = nullptr;
		return this;
	}
	
	return nullptr;
}


void ABaseWeapon::CheckHeroCharacter(ABaseProjectile* Projectile) {
	AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(Character);
	if (HeroCharacter) {
		Projectile->Tags.Add("FromPlayer");
		if (!HeroCharacter->destroy_projectiles_on_hit) {
			Projectile->destroy_on_hit = false;
		}
	}
	else {
		Projectile->Tags.Add("FromEnemy");
	}
}
