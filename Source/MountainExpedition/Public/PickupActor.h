// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "PickupActor.generated.h"

/**
 * 
 */
UCLASS()
class MOUNTAINEXPEDITION_API APickupActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	APickupActor();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItem> ItemToPickup;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	float SpeedRotation;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PickupSound;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	void BeginInteract(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	virtual void BeginInteract_Implementation(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};
