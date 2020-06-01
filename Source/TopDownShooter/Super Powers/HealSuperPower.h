// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Super Powers/BaseSuperPower.h"
#include "HealSuperPower.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AHealSuperPower : public ABaseSuperPower
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AHealSuperPower();

	void Execute(AHeroCharacter* NewCharacter) override;

};
