// Fill out your copyright notice in the Description page of Project Settings.


#include "Observer.h"

// Sets default values
AObserver::AObserver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObserver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObserver::OnNotify(AHeroCharacter* character, E_ObserverEvent event) {
	check(0 && "You must override this");
}

