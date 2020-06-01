// Fill out your copyright notice in the Description page of Project Settings.


#include "KillSuperPower.h"
#include "./Characters/HeroCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h" 

AKillSuperPower::AKillSuperPower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass>FoundParticles(TEXT("BlueprintGeneratedClass'/Game/Blueprints/SuperPowers/BP_Particle_Kill.BP_Particle_Kill_C'"));
	if (FoundParticles.Succeeded())
	{
		ParticlesClass = FoundParticles.Object;
	}

	Type = E_SuperPowerType::EActive;
	Name = E_SuperPowerName::EKill;
	reload_time = 60;

}

// Called when the game starts or when spawned
void AKillSuperPower::BeginPlay()
{
	Super::BeginPlay();

}

void AKillSuperPower::Execute(AHeroCharacter* NewCharacter) {
	Character = NewCharacter;

	active = true;

	FVector location = Character->GetActorLocation();
	location.Z -= 90.15;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Particles = GetWorld()->SpawnActor<AActor>(ParticlesClass, FTransform(FRotator(0, 0, 0), location, FVector(1, 1, 1)), SpawnInfo);
	
	Super::Execute(Character);
}
