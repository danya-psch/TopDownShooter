// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "./Weapons/DroppedWeapon.h"
#include "./Weapons/BaseWeapon.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Engine/Classes/Animation/AnimMontage.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "./Super Powers/BaseSuperPower.h"
#include "HeroCharacter.generated.h"


/**
 *
 */

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class E_ObserverEvent : uint8
{
	EKILL_ENEMIES 	UMETA(DisplayName = "KILL_ENEMIES")
};

class AObserver;

UCLASS()
class TOPDOWNSHOOTER_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
		USpringArmComponent* SelfieStick;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
		UCameraComponent* HeroCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		TSubclassOf<class ABaseWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		TArray<ABaseWeapon*> Weapons;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		int32 CurrentWeaponIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		int32 MaxWeaponIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		ADroppedWeapon* WeaponForTake;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UClass* AnimInstanceClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuperPower")
		ABaseSuperPower* HeroSuperPower;

	bool destroy_projectiles_on_hit = true;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
	//	class UParticleSystemComponent* ParticleSystem;

	// Sets default values for this character's properties
	AHeroCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void MoveUp(float val);

	UFUNCTION()
		void MoveRight(float val);

	UFUNCTION()
		void Attack(float val);

	UFUNCTION()
		void ActivateSuperPower();

	UFUNCTION()
		void TakeItem();

	UFUNCTION()
		void ThrowOutTheItem();

	//Weapon
	UFUNCTION()
		void SwitchWeapon(int weapon_index);

	UFUNCTION()
		void TakeTheNextWeapon();

	UFUNCTION()
		void TakeThePreviousWeapon();

	//Animation
	UFUNCTION(BlueprintCallable)
		void SetAnimInstance(UAnimInstance* AnimI);

	UFUNCTION(BlueprintCallable)
		void AnimationRequest(UAnimMontage* AnimMontage);

	//SuperPower
	UFUNCTION(BlueprintCallable)
	void ChangeSuperPower(ABaseSuperPower* NewSuperPower);

	UFUNCTION()
		void ToMainMenu();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void OnNotify(E_ObserverEvent Event);

	void AddObserver(AObserver* Observer);

	void RemoveObserver(AObserver* Observer);

	void Death() override;
	
	//IDamageable
	void AffectHealth_Implementation(float Delta) override;
private:

	TArray<AObserver*> Observers;
};