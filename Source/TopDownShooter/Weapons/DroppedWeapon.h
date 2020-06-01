// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseWeapon.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroppedWeapon.generated.h"

UCLASS()
class TOPDOWNSHOOTER_API ADroppedWeapon : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		ABaseWeapon* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Death")
		int time_before_death = 20;

	bool selected = false;

	FTimerHandle DelayBeforeDeath;

	void Death();

public:
	// Sets default values for this actor's properties
	ADroppedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void Init(ABaseWeapon* weapon_for_attach);

};
