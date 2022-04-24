// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DirectionalLight.h"

#include "DayNightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOUNTAINEXPEDITION_API UDayNightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDayNightComponent();

	// Current world time in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentTime;

	// Time of full day-night cycle in minutes
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float DayNightLength;

	// Sun Directional Light Reference
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ADirectionalLight* SunLight;

	// Sky Light Reference
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ASkyLight* SkyLight;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Return Hours and Minutes
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetWorldTime(int& Hours, int& Minutes);

	UFUNCTION(BlueprintCallable)
	void AddWorldTime(int Hours);
};
