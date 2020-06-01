// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSuperPower.h"
#include "./Characters/HeroCharacter.h"

// Sets default values
ABaseSuperPower::ABaseSuperPower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	active = false;
}

// Called when the game starts or when spawned
void ABaseSuperPower::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseSuperPower::Init(TSubclassOf<class AActor> NewParticlesClass) {
	ParticlesClass = NewParticlesClass;
}

// Called every frame
void ABaseSuperPower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSuperPower::Reload() {
	//GLog->Log("Reload");
	active = false;
}

void ABaseSuperPower::Disable() {
	if (Particles) {
		Particles->Destroy();
	}
	Destroy();
}

void ABaseSuperPower::Execute(AHeroCharacter* NewCharacter) {
	GetWorld()->GetTimerManager().SetTimer(DelayBeforeDeath, this, &ABaseSuperPower::Reload, 30, false);
}

