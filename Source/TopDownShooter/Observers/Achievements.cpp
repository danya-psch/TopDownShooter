// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievements.h"
#include "Runtime/Core/Public/Misc/OutputDevice.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"

AAchievements::AAchievements() {
	PrimaryActorTick.bCanEverTick = true;
}

void AAchievements::BeginPlay()
{
	Super::BeginPlay();
	GameMode = GetWorld()->GetAuthGameMode();

}

void AAchievements::OnNotify(AHeroCharacter* character, E_ObserverEvent event) {
	//GLog->Log("AAchievements_OnNotify");
	switch (event)
	{
	case E_ObserverEvent::EKILL_ENEMIES :
		dead_enemies++;
		switch (dead_enemies)
		{
		case 5 :
			Unlock(E_Achievement::EKILL_5_ENEMIES);
			break;
		case 20 :
			Unlock(E_Achievement::EKILL_20_ENEMIES);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void AAchievements::Unlock(E_Achievement achievement) {

	if (Achievements.Contains(achievement) != true) {
		Achievements.Add(achievement);

		//GLog->Log("AAchievements_Unlock");
		FOutputDeviceNull ar;
		const FString command = FString::Printf(TEXT("ShowAchievement %d"), achievement);
		GameMode->CallFunctionByNameWithArguments(*command, ar, NULL, true);
		
	}
}
