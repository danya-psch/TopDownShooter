// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Weapons/BaseWeapon.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "CoreMinimal.h"
#include "./Interfaces/Damageable.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API ABaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
public:

	//Make a health property
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Health = 100;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float MaxHealth = 100;

	//Make an isDead property
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
		bool isDead = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		bool dies = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		ABaseWeapon* Weapon;

	FTimerHandle DelayBeforeDeath;

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Calculate death function(helper)
	virtual void CalculateDead();

	virtual void Death();

	//Calculate health function
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
		virtual void CalculateHealth(float delta);

	//Editor-centric code for changing properties
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
		override;
#endif

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		void AffectHealth(float Delta);
		virtual void AffectHealth_Implementation(float Delta);
};
