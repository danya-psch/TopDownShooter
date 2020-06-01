// Fill out your copyright notice in the Description page of Project Settings.


#include "BlizzardSuperPower.h"
#include "./Characters/HeroCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h" 

ABlizzardSuperPower::ABlizzardSuperPower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass>FoundParticles(TEXT("BlueprintGeneratedClass'/Game/Blueprints/SuperPowers/BP_Particle_Heal.BP_Particle_Heal_C'"));
	if (FoundParticles.Succeeded())
	{
		ParticlesClass = FoundParticles.Object;
	}

	Type = E_SuperPowerType::EActive;
}

// Called when the game starts or when spawned
void ABlizzardSuperPower::BeginPlay()
{
	Super::BeginPlay();

}

void ABlizzardSuperPower::Execute(AHeroCharacter* Character) {
	active = true;

	FVector location = Character->GetActorLocation();
	location.Z = 0;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Particles = GetWorld()->SpawnActor<AActor>(ParticlesClass, FTransform(FRotator(0, 0, 0), location, FVector(1, 1, 1)), SpawnInfo);
}
