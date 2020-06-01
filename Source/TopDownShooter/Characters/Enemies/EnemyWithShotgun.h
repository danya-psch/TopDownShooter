// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/EnemyCharacter.h"
#include "EnemyWithShotgun.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AEnemyWithShotgun : public AEnemyCharacter
{
	GENERATED_BODY()
public:
	AEnemyWithShotgun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AEnemyCharacter* Clone(FTransform transform) override;
	
};
