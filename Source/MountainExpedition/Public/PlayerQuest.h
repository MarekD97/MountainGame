// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerQuest.generated.h"

UCLASS()
class MOUNTAINEXPEDITION_API APlayerQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerQuest();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Quest")
		FText Title;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Quest", meta = (MultiLine = true))
		FText Content;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Quest")
		UTexture2D* Image;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Quest")
		uint8 bEnabled:1;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Quest")
		class APlayerQuest* NextQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
		class USoundBase* EnterSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Cylinder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBillboardComponent* Sprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetActive(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
		void ShowQuest();

	UFUNCTION(BlueprintCallable)
		void OnComponentBeginOverlap
		(
			UPrimitiveComponent* OverlappedComponent, 
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult
		);



};
