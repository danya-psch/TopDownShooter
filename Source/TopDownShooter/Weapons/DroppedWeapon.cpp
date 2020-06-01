// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppedWeapon.h"
#include "./Characters/HeroCharacter.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

// Sets default values
ADroppedWeapon::ADroppedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = CollisionComp;
	CollisionComp->SetWorldScale3D(FVector(2, 2, 2));

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADroppedWeapon::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ADroppedWeapon::OnOverlapEnd);

	this->Tags.Add(FName("DroppedWeapon"));
}

// Called when the game starts or when spawned
void ADroppedWeapon::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(DelayBeforeDeath, this, &ADroppedWeapon::Death, time_before_death, false);
}

// Called every frame
void ADroppedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + 1, GetActorRotation().Roll));

}

void ADroppedWeapon::Init(ABaseWeapon* weapon_for_attach) {
	if (weapon_for_attach) {
		Weapon = weapon_for_attach;
		Weapon->AttachToComponent(CollisionComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SetActorRotation(FRotator(0, 0, -10));
		if (selected) {
			Weapon->WeaponMesh->SetRenderCustomDepth(true);
		}
	}
}

void ADroppedWeapon::Death() {
	if (Weapon) {
		Weapon->WeaponMesh->SetRenderCustomDepth(false);
	}
	Weapon->Destroy();
	Destroy();
}

void ADroppedWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player") && ((AHeroCharacter*)OtherActor)->WeaponForTake == nullptr) {
		((AHeroCharacter*)OtherActor)->WeaponForTake = this;
		selected = true;
		if (Weapon) {
			Weapon->WeaponMesh->SetRenderCustomDepth(true);
		}
	}
}

void ADroppedWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player") && ((AHeroCharacter*)OtherActor)->WeaponForTake == this) {
		((AHeroCharacter*)OtherActor)->WeaponForTake = nullptr;
		selected = false;
		if (Weapon) {
			Weapon->WeaponMesh->SetRenderCustomDepth(false);
		}
	}
}
