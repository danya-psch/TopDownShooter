// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "./Projectiles/BaseProjectile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseCharacter;

UENUM(BlueprintType)
enum E_WeaponType
{
	EShotgun 		UMETA(DisplayName = "Shotgun"),
	ERifle 	        UMETA(DisplayName = "Rifle"),
	EPistol 		UMETA(DisplayName = "Pistol"),
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ammo")
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 BurstCount;

	UPROPERTY(EditAnywhere, Category = "Ammo")
		int32 ShotCost;

	UPROPERTY(EditAnywhere, Category = "Config")
		float TimeBetweenShots;

	UPROPERTY(EditAnywhere, Category = "Config")
		float WeaponRange;

	UPROPERTY(EditAnywhere, Category = "Config")
		float WeaponSpread;

	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<class AActor> projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		TEnumAsByte<E_WeaponType> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundOfShot;

};

UCLASS()
class TOPDOWNSHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		FWeaponData WeaponConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
		UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		ABaseCharacter* Character;

	//TimerForAttack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool trigger_pulled;

	FTimerHandle DelayBetweenShooting;
	//

	//TimerForDelayAfterShooting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool ability_to_shoot;

	FTimerHandle DelayAfterShooting;
	//



public:
	// Sets default values for this actor's properties
	ABaseWeapon();
	/*
	ABaseWeapon(const FObjectInitializer& ObjectInitializer);
	*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void Fire();

	UFUNCTION(BlueprintCallable)
		void PullTrigger();

	UFUNCTION(BlueprintCallable)
		void ReleaseTrigger();

	UFUNCTION(BlueprintCallable)
		void RecoveryAbilityToShoot();

	UFUNCTION(BlueprintCallable)
		void AttachToPlayer(ABaseCharacter* NewCharacter);

	UFUNCTION(BlueprintCallable)
		ABaseWeapon* DetachFromPlayer();

	void CheckHeroCharacter(ABaseProjectile* Projectile);


};
