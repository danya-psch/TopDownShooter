// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSuperPower.generated.h"

class AHeroCharacter;


UENUM(BlueprintType)
enum E_SuperPowerType
{
	EActive 		UMETA(DisplayName = "Active"),
	EPassive 	    UMETA(DisplayName = "Passive"),
};

UENUM(BlueprintType)
enum E_SuperPowerName
{
	EHeal 				UMETA(DisplayName = "Heal"),
	EKill 				UMETA(DisplayName = "Kill"),
	ESpecProjectile		UMETA(DisplayName = "SpecProjectile"),
};

UCLASS()
class TOPDOWNSHOOTER_API ABaseSuperPower : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ABaseSuperPower();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
		TSubclassOf<class AActor> ParticlesClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
		AActor* Particles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuperPower")
	bool active;

	int reload_time;

	FTimerHandle DelayBeforeDeath;

	E_SuperPowerType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<E_SuperPowerName> Name;

	AHeroCharacter* Character;

	void Reload();

	UFUNCTION(BlueprintCallable)
		void Init(TSubclassOf<class AActor> NewParticlesClass);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Disable();

	virtual void Execute(AHeroCharacter* Character);

};
