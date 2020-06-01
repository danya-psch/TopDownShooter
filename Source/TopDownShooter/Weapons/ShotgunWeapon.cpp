// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"
#include "./Projectiles/BaseProjectile.h"
#include "./Characters/HeroCharacter.h"
#include "Runtime/Engine/Classes/Engine/SkeletalMesh.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AShotgunWeapon::AShotgunWeapon() {
	WeaponConfig.WeaponType = E_WeaponType::EShotgun;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>FoundMesh(TEXT("SkeletalMesh'/Game/WeaponComponent/UnnecessaryContent/Weapons/Mesh/SK_FPGun.SK_FPGun'"));
	if (FoundMesh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(FoundMesh.Object);
	}

	WeaponConfig.BurstCount = 10;
	WeaponConfig.WeaponSpread = 10;
	WeaponConfig.TimeBetweenShots = 1.3;

	ProjectileSpawnPoint->SetRelativeLocation(FVector(0, 56, 11));
	ProjectileSpawnPoint->SetRelativeRotation(FRotator(0, 90, 0));

	static ConstructorHelpers::FObjectFinder<UClass>FoundProjectile(TEXT("BlueprintGeneratedClass'/Game/Blueprints/Projectiles/ShotgunProjectile.ShotgunProjectile_C'"));
	if (FoundProjectile.Succeeded())
	{
		WeaponConfig.projectile = FoundProjectile.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>FoundSound(TEXT("SoundWave'/Game/Audio/Weapons/Shotgun_Shot.Shotgun_Shot'"));
	if (FoundSound.Succeeded())
	{
		WeaponConfig.SoundOfShot = FoundSound.Object;
	}
}

void AShotgunWeapon::Fire() {

	//UE_LOG(LogTemp, Warning, TEXT("Fire"));

	for (int i = 0; i < 10; i++) { //WeaponConfig.BurstCount
		FRotator rotation = FRotator(FMath::FRandRange(-1 * WeaponConfig.WeaponSpread, WeaponConfig.WeaponSpread),
			FMath::FRandRange(-1 * WeaponConfig.WeaponSpread, WeaponConfig.WeaponSpread),
			FMath::FRandRange(-1 * WeaponConfig.WeaponSpread, WeaponConfig.WeaponSpread)
		);


		FTransform old_transform = ProjectileSpawnPoint->GetComponentTransform();
		FTransform new_transform = FTransform(rotation + old_transform.GetRotation().Rotator(),
			old_transform.GetLocation(), FVector(1, 1, 1));

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(WeaponConfig.projectile, new_transform, SpawnInfo);

		CheckHeroCharacter(Projectile);

	}

	if (WeaponConfig.SoundOfShot) UGameplayStatics::SpawnSoundAttached(WeaponConfig.SoundOfShot, ProjectileSpawnPoint);
}