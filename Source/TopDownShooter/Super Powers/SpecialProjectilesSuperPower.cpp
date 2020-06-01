// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialProjectilesSuperPower.h"
#include "./Characters/HeroCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h" 

ASpecialProjectilesSuperPower::ASpecialProjectilesSuperPower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FObjectFinder<UClass>FoundParticles(TEXT("BlueprintGeneratedClass'/Game/Blueprints/BP_Particle_Heal.BP_Particle_Heal_C'"));
	if (FoundParticles.Succeeded())
	{
		ParticlesClass = FoundParticles.Object;
	}*/

	Type = E_SuperPowerType::EPassive;
	Name = E_SuperPowerName::ESpecProjectile;
}

// Called when the game starts or when spawned
void ASpecialProjectilesSuperPower::BeginPlay()
{
	Super::BeginPlay();

}

void ASpecialProjectilesSuperPower::Disable() {
	if (Character) {
		Character->destroy_projectiles_on_hit = true;
		Character->HeroSuperPower = nullptr;
	}
	if (Particles) {
		Particles->Destroy();
	}
	Destroy();
}

void ASpecialProjectilesSuperPower::Execute(AHeroCharacter* NewCharacter) {
	Character = NewCharacter;
	active = true;
	Character->destroy_projectiles_on_hit = false;
}
