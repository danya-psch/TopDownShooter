// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "ShotgunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API AShotgunWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	AShotgunWeapon();

	void Fire() override;
};
