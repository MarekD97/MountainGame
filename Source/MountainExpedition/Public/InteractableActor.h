// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class MOUNTAINEXPEDITION_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OverlapRadius;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginInteract(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndInteract(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		class UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	

public:	

};
