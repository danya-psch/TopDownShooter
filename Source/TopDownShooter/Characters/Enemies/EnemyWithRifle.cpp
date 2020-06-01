// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWithRifle.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Math/Color.h"

AEnemyWithRifle::AEnemyWithRifle() {
	static ConstructorHelpers::FObjectFinder<UClass>FoundWeapon(TEXT("BlueprintGeneratedClass'/Game/Blueprints/Weapons/RifleWeapon.RifleWeapon_C'"));
	if (FoundWeapon.Succeeded())
	{
		WeaponClass = FoundWeapon.Object;
	}

	EnemyColor = FLinearColor(1, 0.1, 0.2);
}


void AEnemyWithRifle::BeginPlay() {
	Super::BeginPlay();
}


AEnemyCharacter* AEnemyWithRifle::Clone(FTransform transform) {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyWithRifle>(GetClass(), transform, SpawnInfo);
	return Enemy;
}


