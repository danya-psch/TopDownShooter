// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpawner.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "./Characters/HeroCharacter.h"

// Sets default values
ABaseSpawner::ABaseSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	//Trigger->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseSpawner::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ABaseSpawner::OnOverlapEnd);
	Trigger->SetRelativeLocation(FVector(550, 0, 88));
	Trigger->SetSphereRadius(1000);
	Trigger->ComponentTags.Add("SpawnerTrigger");

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	//SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SpawnVolume->SetupAttachment(RootComponent);
	SpawnVolume->SetGenerateOverlapEvents(false);
	SpawnVolume->SetRelativeLocation(FVector(550, 0, 88));
	SpawnVolume->SetBoxExtent(FVector(400, 400, 20));

	complexity = E_LevelOfDifficulty::EMedium;


}

// Called when the game starts or when spawned
void ABaseSpawner::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
}

// Called every frame
void ABaseSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseSpawner::SpawnEnemy() {
	//GLog->Log("SpawnEnemy");
	if (Enemy && (!take_account_of_waves || (number_of_spawns < number_of_waves))) {
		for (int i = 0; i < (number_of_spawns / (int)complexity) + 1; i++) {
			//GLog->Log("Spawn");
			FVector location = UKismetMathLibrary::RandomPointInBoundingBox(SpawnVolume->GetComponentLocation(), SpawnVolume->GetScaledBoxExtent());
			location.Z = 88;
			AEnemyCharacter* NewEnemy = Enemy->Clone(FTransform(FRotator(0, 0, 0), location, FVector(1, 1, 1)));
			if (NewEnemy) NewEnemy->enemy_is_seen = true;
		}
		number_of_spawns++;
	}
}

bool take_account_of_waves = false;
void ABaseSpawner::Init(AEnemyCharacter* NewEnemy, int32 number_of_waves_, bool take_account_of_waves_, E_LevelOfDifficulty complexity_) {
	if (NewEnemy) Enemy = NewEnemy;
	number_of_waves = number_of_waves_;
	take_account_of_waves = take_account_of_waves_;
	complexity = complexity_;
}

void ABaseSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Cast<AHeroCharacter>(OtherActor)) {
		//GLog->Log("OnOverlapBegin");
		SpawnEnemy();
		GetWorld()->GetTimerManager().SetTimer(DelayBetweenSpawning, this, &ABaseSpawner::SpawnEnemy, 10.0f, true);
	}
}

void ABaseSpawner::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (Cast<AHeroCharacter>(OtherActor)) {
		//GLog->Log("OnOverlapEnd");
		GetWorld()->GetTimerManager().ClearTimer(DelayBetweenSpawning);
	}
}



