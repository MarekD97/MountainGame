// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	OverlapRadius = 64.0f;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->InitSphereRadius(OverlapRadius);
	RootComponent = SphereCollision;
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::BeginInteract);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::EndInteract);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableActor::BeginInteract_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GLog->Log("Begin Interact");
}

void AInteractableActor::EndInteract_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	GLog->Log("End interact");
}

