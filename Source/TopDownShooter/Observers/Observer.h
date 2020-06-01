// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./Characters/HeroCharacter.h"
#include "GameFramework/Actor.h"
#include "Observer.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API AObserver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObserver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnNotify(AHeroCharacter* character, E_ObserverEvent event);
};
