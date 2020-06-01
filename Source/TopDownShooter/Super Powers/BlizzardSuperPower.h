// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Super Powers/BaseSuperPower.h"
#include "BlizzardSuperPower.generated.h"

/**
 *
 */
UCLASS()
class TOPDOWNSHOOTER_API ABlizzardSuperPower : public ABaseSuperPower
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ABlizzardSuperPower();

	void Execute(AHeroCharacter* Character) override;

};