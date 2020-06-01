// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroCharacter.h"
#include "./Weapons/DroppedWeapon.h"
#include "./Super Powers/HealSuperPower.h"
#include "./Super Powers/KillSuperPower.h"
#include "./Super Powers/SpecialProjectilesSuperPower.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Runtime/Core/Public/Math/Vector.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "./Observers/Achievements.h"


// Sets default values
AHeroCharacter::AHeroCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentWeaponIndex = 0;

	//Camera setting
	USpringArmComponent* SelfieStick = CreateDefaultSubobject<USpringArmComponent>(TEXT("SelfieStick"));
	//SelfieStick->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SelfieStick->SetupAttachment(RootComponent);

	SelfieStick->TargetArmLength = 400;//300
	SelfieStick->SetRelativeRotation(FRotator(-70, 0, 0));
	SelfieStick->bDoCollisionTest = false;
	SelfieStick->bInheritPitch = false;
	SelfieStick->bInheritRoll = false;
	SelfieStick->bInheritYaw = false;

	UCameraComponent* HeroCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroCamera"));
	//HeroCamera->AttachToComponent(SelfieStick, FAttachmentTransformRules::KeepRelativeTransform);
	HeroCamera->SetupAttachment(SelfieStick);
	HeroCamera->SetRelativeLocation(FVector(-350, 0, 0));

	static ConstructorHelpers::FObjectFinder<UClass>FoundWeapon(TEXT("BlueprintGeneratedClass'/Game/Blueprints/Weapons/ShotgunWeapon.ShotgunWeapon_C'"));
	if (FoundWeapon.Succeeded())
	{
		WeaponClass = FoundWeapon.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UClass>AnimClass(TEXT("AnimBlueprintGeneratedClass'/Game/MyAnim/HeroAnimBP.HeroAnimBP_C'"));
	if (AnimClass.Succeeded())
	{
		AnimInstanceClass = AnimClass.Object;
	}
	
	this->Tags.Add(FName("Player"));

	//Observers

	//addObserver(new Achievements());

	
	/*ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>FoundParticles(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (FoundParticles.Succeeded())
	{
		ParticleSystem->SetTemplate(FoundParticles.Object);
	}*/
	MaxWeaponIndex = 2;


}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn Weapon
	/*FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseWeapon* BaseWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, GetMesh()->GetComponentTransform(), SpawnInfo);
	BaseWeapon->AttachToPlayer(this);
	*/

	if (AnimInstanceClass) {
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass);
	}

	//Spawn SuperPower
	//ChangeSuperPower(GetWorld()->SpawnActor<ASpecialProjectilesSuperPower>(SpawnInfo));

	//Spawn Observer
	FActorSpawnParameters SpawnInfo;
	AddObserver(GetWorld()->SpawnActor<AAchievements>(SpawnInfo));

}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isDead)
	{

		APlayerController * controller = GetWorld()->GetFirstPlayerController();
		if (Weapon) {
			FHitResult TraceResult(ForceInit);
			controller->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, TraceResult);

			if (TraceResult.ImpactPoint.Z < GetActorLocation().Z * 4) {
				FRotator directionCharToCursor = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), TraceResult.ImpactPoint);

				FRotator newRotation = FMath::RInterpTo(this->GetControlRotation(), FRotator(0, directionCharToCursor.Yaw, 0), DeltaTime, 10);

				controller->SetControlRotation(newRotation);
			}
		}
		else if (GetVelocity() != UKismetMathLibrary::Vector_Zero()) {
			FRotator directionCharToVelocity = UKismetMathLibrary::MakeRotFromX(GetVelocity());
			
			FRotator newRotation = FMath::RInterpTo(this->GetControlRotation(), FRotator(0, directionCharToVelocity.Yaw, 0), DeltaTime, 10);

			controller->SetControlRotation(newRotation);
		}
	}
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &AHeroCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHeroCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Attack", this, &AHeroCharacter::Attack);

	PlayerInputComponent->BindAction("TakeItem", IE_Pressed, this, &AHeroCharacter::TakeItem);
	PlayerInputComponent->BindAction("ThrowOutTheItem", IE_Pressed, this, &AHeroCharacter::ThrowOutTheItem);

	PlayerInputComponent->BindAction("TakeTheNextWeapon", IE_Pressed, this, &AHeroCharacter::TakeTheNextWeapon);
	PlayerInputComponent->BindAction("TakeThePreviousWeapon", IE_Pressed, this, &AHeroCharacter::TakeThePreviousWeapon);

	PlayerInputComponent->BindAction("ActivateSuperPower", IE_Pressed, this, &AHeroCharacter::ActivateSuperPower);

	PlayerInputComponent->BindAction("ToMainMenu", IE_Pressed, this, &AHeroCharacter::ToMainMenu);
}

void AHeroCharacter::MoveUp(float value)
{
	if (!isDead) AddMovementInput(FVector(1, 0, 0), value);
}

void AHeroCharacter::MoveRight(float value)
{
	if (!isDead) AddMovementInput(FVector(0, 1, 0), value);
}

void AHeroCharacter::Attack(float value)
{

	if (Weapon)
	{
		if (value == 1 && !isDead)
		{
			Weapon->PullTrigger();
		}
		else
		{
			Weapon->ReleaseTrigger();
		}

		if (isDead) {
			Weapon->ReleaseTrigger();
		}
	}

}

//SuperPower
void AHeroCharacter::ActivateSuperPower() {
	if (HeroSuperPower && !HeroSuperPower->active && HeroSuperPower->Type != E_SuperPowerType::EPassive) {
		//GLog->Log("ActivateSuperPower");
		HeroSuperPower->Execute(this);
	}
}

//interaction with items
void AHeroCharacter::TakeItem()
{
	if (!isDead && WeaponForTake) {
		
		WeaponForTake->Weapon->WeaponMesh->SetRenderCustomDepth(false);
		WeaponForTake->Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		
		ABaseWeapon* NewWeapon = WeaponForTake->Weapon;
		WeaponForTake->Destroy();

		if (Weapons.Num() == MaxWeaponIndex) {
			ThrowOutTheItem();
		}
		else if (Weapon) {
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("WeaponBehindSocket")));
		}

		if (NewWeapon) NewWeapon->AttachToPlayer(this);

	}
}

void AHeroCharacter::ThrowOutTheItem() {
	if (!isDead && Weapon) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ADroppedWeapon* DroppedWeapon = GetWorld()->SpawnActor<ADroppedWeapon>(GetActorLocation(), GetActorRotation(), SpawnInfo);
		DroppedWeapon->Init(Weapon->DetachFromPlayer());
		//if (WeaponForTake == DroppedWeapon && WeaponForTake->Weapon) {
		//	DroppedWeapon->Weapon->WeaponMesh->SetRenderCustomDepth(true);
		//}
	}
	
}
//

//SwitchWeapon

void AHeroCharacter::SwitchWeapon(int weapon_index) {
	if (Weapons.IsValidIndex(weapon_index)) {
		if (Weapon) {
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("WeaponBehindSocket")));
		}

		Weapon = Weapons[weapon_index];

		Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("GunSocket")));
	}
}

void AHeroCharacter::TakeTheNextWeapon() {
	if (!isDead) {
		if (CurrentWeaponIndex < Weapons.Num() - 1) {
			CurrentWeaponIndex = CurrentWeaponIndex + 1;
		}
		else {
			CurrentWeaponIndex = 0;
		}
		SwitchWeapon(CurrentWeaponIndex);
	}
}

void AHeroCharacter::TakeThePreviousWeapon() {
	if (!isDead) {
		if (CurrentWeaponIndex > 0) {
			CurrentWeaponIndex = CurrentWeaponIndex - 1;
		}
		else {
			CurrentWeaponIndex = Weapons.Num() - 1;
		}
		SwitchWeapon(CurrentWeaponIndex);
	}
}

//Animation
void AHeroCharacter::SetAnimInstance(UAnimInstance* AnimI) {
	//AnimInstance = AnimI;
}

void AHeroCharacter::AnimationRequest(UAnimMontage* AnimMontage) {
	//AnimInstance->Montage_Play(AnimMontage, 1);
}

//SuperPower
void AHeroCharacter::ChangeSuperPower(ABaseSuperPower* NewSuperPower) {
	if (NewSuperPower) {

		if (HeroSuperPower) HeroSuperPower->Disable();

		HeroSuperPower = NewSuperPower;
		HeroSuperPower->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		if (NewSuperPower->Type == E_SuperPowerType::EPassive) HeroSuperPower->Execute(this);
		
		FOutputDeviceNull ar;
		const FString command = FString::Printf(TEXT("SuperPowerChange %d"), HeroSuperPower->Name);
		GetWorld()->GetAuthGameMode()->CallFunctionByNameWithArguments(*command, ar, NULL, true);
	}
}

void AHeroCharacter::ToMainMenu() {
	UGameplayStatics::OpenLevel(this, FName("MainMenuMap"), true);
}

//Observer
void AHeroCharacter::AddObserver(AObserver* Observer) {
	if (Observer) {
		Observer->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Observers.Add(Observer);
	}
}

void AHeroCharacter::RemoveObserver(AObserver* Observer) {
	if (Observer) {
		Observers.Remove(Observer);
	}
}

void AHeroCharacter::Death() {

	/*for (int i = 0; i < Weapons.Num(); i++) {
		Weapons[i]->Destroy();
	}
	Destroy();*/
}

void AHeroCharacter::OnNotify(E_ObserverEvent Event) {
	for (AObserver* Observer : Observers) {
		Observer->OnNotify(this, Event);
	}
}


void AHeroCharacter::AffectHealth_Implementation(float Delta) {
	CalculateHealth(Delta);
	if (isDead && !dies) {
		dies = true;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
		GetWorld()->GetTimerManager().SetTimer(DelayBeforeDeath, this, &AHeroCharacter::Death, 2.4f, false);
		
		FOutputDeviceNull ar;
		GetWorld()->GetAuthGameMode()->CallFunctionByNameWithArguments(TEXT("EndOfTheGame"), ar, NULL, true);
	}
}
