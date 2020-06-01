// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWithShotgun.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Math/Color.h"

AEnemyWithShotgun::AEnemyWithShotgun() {
	static ConstructorHelpers::FObjectFinder<UClass>FoundWeapon(TEXT("BlueprintGeneratedClass'/Game/Blueprints/Weapons/ShotgunWeapon.ShotgunWeapon_C'"));
	if (FoundWeapon.Succeeded())
	{
		WeaponClass = FoundWeapon.Object;
	}

	EnemyColor = FLinearColor(1, 0.6, 0.3);
}


void AEnemyWithShotgun::BeginPlay() {
	Super::BeginPlay();
}


AEnemyCharacter* AEnemyWithShotgun::Clone(FTransform transform) {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyWithShotgun>(GetClass(), transform, SpawnInfo);
	return Enemy;
}
