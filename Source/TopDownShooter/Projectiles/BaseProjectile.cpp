// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "./Interfaces/Damageable.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "./Characters/EnemyCharacter.h"
#include "./Characters/HeroCharacter.h"
#include "Runtime/Core/Public/Internationalization/Text.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	//ProjectileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ProjectileMesh->SetupAttachment(RootComponent);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	//CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 1500;

	InitialLifeSpan = 1.5;

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);
	
	//ParticleSystem'/Game/Materials/LaserSparks_2.LaserSparks_2'

	static ConstructorHelpers::FObjectFinder<UParticleSystem>FoundParticles(TEXT("ParticleSystem'/Game/Materials/LaserSparks.LaserSparks'"));
	if (FoundParticles.Succeeded())
	{
		Particles = FoundParticles.Object;
	}
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!Cast<ABaseProjectile>(OtherActor) &&
		!OtherActor->Tags.Contains("DroppedWeapon") &&
		!OtherActor->Tags.Contains("SuperPower") &&
		!OtherActor->Tags.Contains("SuperPowerPrefab") &&
		!OtherComp->ComponentTags.Contains("SpawnerTrigger")
		) {
		if (OtherActor->Tags.Contains("SuperPowerPrefab")) {
			//GLog->Log("SuperPowerPrefab");
		}
		IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);

		if (DamageableActor && !(Tags.Contains("FromEnemy") && Cast<AEnemyCharacter>(OtherActor))) { //DamageableActor && !((Tags.Contains("FromEnemy") || Tags.Num() == 0) && Cast<AEnemyCharacter>(OtherActor))
			DamageableActor->Execute_AffectHealth(OtherActor, -1 * damage);
		}

		SpawnParticles();

		if (destroy_on_hit) {
			GetWorld()->GetTimerManager().SetTimer(DelayBeforeDeath, this, &ABaseProjectile::Death, 0.002f, false);
		}
	}
}

void ABaseProjectile::Death() {
	Destroy();
	
}

void ABaseProjectile::SpawnParticles() {
	if (Particles) {
		FTransform transform = FTransform(GetActorRotation(), GetActorLocation(), FVector(1, 1, 1));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particles, transform, true);
	}
}
