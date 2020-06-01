// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Core/Public/Containers/Map.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "CoreMinimal.h"
#include "Observers/Observer.h"
#include "Achievements.generated.h"

/**
 * 
 */

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class E_Achievement : uint8
{
	EKILL_5_ENEMIES 	UMETA(DisplayName = "KILL_5_ENEMIES"),
	EKILL_20_ENEMIES 	UMETA(DisplayName = "KILL_20_ENEMIES")
};

UCLASS()
class TOPDOWNSHOOTER_API AAchievements : public AObserver
{
	GENERATED_BODY()

public:
	AAchievements();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		AGameModeBase* GameMode;

	void OnNotify(AHeroCharacter* character, E_ObserverEvent event) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Unlock(E_Achievement achievement);

	TArray<E_Achievement> Achievements;

	int dead_enemies;

	
};
