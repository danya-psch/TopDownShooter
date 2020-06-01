// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleWeapon.h"
#include "./Characters/HeroCharacter.h"
#include "./Projectiles/BaseProjectile.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ARifleWeapon::ARifleWeapon() {
	WeaponConfig.WeaponType = E_WeaponType::ERifle;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>FoundMesh(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (FoundMesh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(FoundMesh.Object);
	}

	WeaponConfig.TimeBetweenShots = 0.233;

	WeaponMesh->SetRelativeScale3D(FVector(1.5, 1.5, 1.5));
	ProjectileSpawnPoint->SetRelativeLocation(FVector(0, 60, 10.5));
	ProjectileSpawnPoint->SetRelativeRotation(FRotator(0, 90, 0));

	static ConstructorHelpers::FObjectFinder<UClass>FoundProjectile(TEXT("BlueprintGeneratedClass'/Game/Blueprints/Projectiles/RifleProjectile.RifleProjectile_C'"));
	if (FoundProjectile.Succeeded())
	{
		WeaponConfig.projectile = FoundProjectile.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>FoundSound(TEXT("SoundWave'/Game/Audio/Weapons/Rifle_Shot.Rifle_Shot'"));
	if (FoundSound.Succeeded())
	{
		WeaponConfig.SoundOfShot = FoundSound.Object;
	}

	//SoundWave'/Game/WeaponComponent/UnnecessaryContent/Audio/FPTWeaponFire02.FPTWeaponFire02'
}


void ARifleWeapon::Fire() {

	//UE_LOG(LogTemp, Warning, TEXT("Fire"));

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(WeaponConfig.projectile, ProjectileSpawnPoint->GetComponentTransform(), SpawnInfo);

	CheckHeroCharacter(Projectile);
	
	if (WeaponConfig.SoundOfShot) UGameplayStatics::SpawnSoundAttached(WeaponConfig.SoundOfShot, ProjectileSpawnPoint);

}
