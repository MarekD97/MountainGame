// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Item.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	SpeedRotation = 45.0f;
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APickupActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MeshComponent->AddLocalRotation(FRotator(0.0f, SpeedRotation*DeltaSeconds, 0.0f));
}

void APickupActor::BeginInteract_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		if(Player)
		{
			UItem* Item = Cast<UItem>(UGameplayStatics::SpawnObject(ItemToPickup, Player));
			Player->InventoryComponent->AddItem(Item);
		}
		
		if(PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
			
			Destroy();
		}
	}

}
