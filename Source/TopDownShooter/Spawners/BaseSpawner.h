// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "./Characters/EnemyCharacter.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpawner.generated.h"

UENUM(BlueprintType)
enum E_LevelOfDifficulty
{
	EEasy = 5  		UMETA(DisplayName = "Easy"),
	EMedium = 3     UMETA(DisplayName = "Medium"),
	EHard = 1 	    UMETA(DisplayName = "Hard")
};

UENUM(BlueprintType)
enum E_SpawnerType
{
	EShotgunEnemy 		UMETA(DisplayName = "ShotgunEnemy"),
	ERifleEnemy	        UMETA(DisplayName = "RifleEnemy"),
};

USTRUCT(BlueprintType)
struct FDataForCreatingSpawners
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<E_SpawnerType> SpawnerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Waves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AccountWaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<E_LevelOfDifficulty> Complexity;
};

UCLASS()
class TOPDOWNSHOOTER_API ABaseSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseSpawner();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
		TSubclassOf<class AEnemyCharacter> EnemyClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy")
		AEnemyCharacter* Enemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spawn")
		UBoxComponent* SpawnVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
		USphereComponent* Trigger;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config")
		int32 number_of_spawns = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config")
		int32 number_of_waves;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Config")
		bool take_account_of_waves = false;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<E_LevelOfDifficulty> complexity;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
		void Init(AEnemyCharacter* NewEnemy,
			int32 number_of_waves_,
			bool take_account_of_waves_,
			E_LevelOfDifficulty complexity_);

	FTimerHandle DelayBetweenSpawning;

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
};
