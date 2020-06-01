// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSHOOTER_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	ARifleWeapon();

	void Fire() override;
};
