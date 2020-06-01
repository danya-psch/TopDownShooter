// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "./Weapons/BaseWeapon.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 *
 */

UCLASS()
class TOPDOWNSHOOTER_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		TSubclassOf<class ABaseWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		UPawnSensingComponent* PawnSensor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UClass* AnimInstanceClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Material")
		UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Material")
		FLinearColor EnemyColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		bool enemy_is_seen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config")
		int32 Score = 5;
	
	AEnemyCharacter();

protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
public:
	
	void Init();

	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);
	
	UFUNCTION(BlueprintCallable)
	virtual AEnemyCharacter* Clone(FTransform transform);

	void Death() override;

	//IDamageable
	void AffectHealth_Implementation(float Delta) override;

};
