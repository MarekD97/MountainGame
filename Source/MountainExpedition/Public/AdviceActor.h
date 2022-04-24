// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "AdviceActor.generated.h"

UCLASS()
class MOUNTAINEXPEDITION_API AAdviceActor : public AInteractableActor
{
	GENERATED_BODY()

	class APlayerCharacter* PlayerRef;

public:
	AAdviceActor();

	UPROPERTY(EditDefaultsOnly)
	FName AdviceText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPaperSpriteComponent* Sprite;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PickupSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MessageText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActionName;

	UFUNCTION(BlueprintCallable)
	FText GetKeyDisplayName(FName Name);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	void BeginInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void BeginInteract_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
