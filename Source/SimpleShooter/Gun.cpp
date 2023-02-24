// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	// Shot Effect
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	if (!GunTrace(Hit, ShotDirection))
	{
		// Trace fail
		return;
	}

	// Impect Effect
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, Hit.Location, ShotDirection.Rotation());
	UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, Hit.Location, ShotDirection.Rotation());

	// Deal Damage
	AActor *HitActor = Hit.GetActor();
	AController *OwnerController = GetOwnerController();
	if (!HitActor || !OwnerController)
	{
		return;
	}

	FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
	HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
}

bool AGun::GunTrace(FHitResult &OutHit, FVector &OutShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if (!OwnerController)
	{
		return false;
	}

	// Shot Start
	FVector StartLocation;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(StartLocation, Rotation);

	// Shot End
	FVector EndLocation = StartLocation + Rotation.Vector() * MaxRange;

	// Shot Direction
	OutShotDirection = -Rotation.Vector();

	// Ignore self & owner
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	// return success
	return GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController *AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		return nullptr;
	}

	return OwnerPawn->GetController();
}
