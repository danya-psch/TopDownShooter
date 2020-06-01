// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "./Characters/HeroCharacter.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AEnemyCharacter::AEnemyCharacter() {
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	this->Tags.Add(FName("Enemy"));
	GetCharacterMovement()->MaxWalkSpeed = 300;

	PawnSensor->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
	PawnSensor->SetPeripheralVisionAngle(60);
	PawnSensor->SightRadius = 1400;

	static ConstructorHelpers::FObjectFinder<UClass>AnimClass(TEXT("AnimBlueprintGeneratedClass'/Game/MyAnim/EnemyAnimBP.EnemyAnimBP_C'"));
	if (AnimClass.Succeeded()) {
		AnimInstanceClass = AnimClass.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UClass>AIClass(TEXT("BlueprintGeneratedClass'/Game/Blueprints/EnemyAI/EnemyAI.EnemyAI_C"));
	if (AIClass.Succeeded()) {
		AIControllerClass = AIClass.Object;
	}*/

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	enemy_is_seen = false;
	EnemyColor = FLinearColor(1, 1, 1);
}

void AEnemyCharacter::BeginPlay() {
	Super::BeginPlay();

	Init();
}

void AEnemyCharacter::Init() {
	if (WeaponClass) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseWeapon* BaseWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, GetMesh()->GetComponentTransform(), SpawnInfo);
		BaseWeapon->AttachToPlayer(this);
	}

	if (AnimInstanceClass) {
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}

	DynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), GetMesh());
	DynamicMaterial->SetVectorParameterValue("BodyColor", EnemyColor);
	GetMesh()->SetMaterial(0, DynamicMaterial);

}

void AEnemyCharacter::OnSeePawn(APawn *OtherPawn) {
	AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(OtherPawn);
	if (HeroCharacter) {
		enemy_is_seen = true;
	}
}

AEnemyCharacter* AEnemyCharacter::Clone(FTransform transform) {
	check(0 && "You must override this");
	return nullptr;
}

void AEnemyCharacter::Death() {
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ADroppedWeapon* DroppedWeapon = GetWorld()->SpawnActor<ADroppedWeapon>(GetActorLocation(), GetActorRotation(), SpawnInfo);
	DroppedWeapon->Init(Weapon->DetachFromPlayer());

	Destroy();
}

void AEnemyCharacter::AffectHealth_Implementation(float Delta) {
	CalculateHealth(Delta);
	if (isDead && !dies) {
		dies = true;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(Character);
		if (HeroCharacter) {
			HeroCharacter->OnNotify(E_ObserverEvent::EKILL_ENEMIES);
		}

		FOutputDeviceNull ar;
		const FString command = FString::Printf(TEXT("IncrementScore %d"), Score);
		GetWorld()->GetAuthGameMode()->CallFunctionByNameWithArguments(*command, ar, NULL, true);

		GetWorld()->GetTimerManager().SetTimer(DelayBeforeDeath, this, &AEnemyCharacter::Death, 2.4f, false);
	}
}