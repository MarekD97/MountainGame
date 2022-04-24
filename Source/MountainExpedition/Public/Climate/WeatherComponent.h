// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "WeatherComponent.generated.h"

UENUM(BlueprintType)
enum ESeasonType
{
	Spring,
	Summer,
	Autumn,
	Winter
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOUNTAINEXPEDITION_API UWeatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeatherComponent();

	// Material Parameter Collection
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Parameter")
	class UMaterialParameterCollection* Collection;
	// Rain Intensity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float RainIntensity;
	// Wind Intensity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float WindIntensity;
	// Snow Intensity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float SnowIntensity;
	// Autumn Intensity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float AutumnIntensity;

	// Rain Particle System
	UPROPERTY(EditInstanceOnly, Category = "Particle")
	class UParticleSystem* RainParticle;
	// Snow Particle System
	UPROPERTY(EditInstanceOnly, Category = "Particle")
	class UParticleSystem* SnowParticle;

	// Rain Sound Base
	UPROPERTY(EditInstanceOnly, Category = "Sound")
	class USoundBase* RainSound;
	// Wind Sound Base
	UPROPERTY(EditInstanceOnly, Category = "Sound")
	class USoundBase* WindSound;

	// World temperature in Celsius
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Temperature")
	float CurrentTemperature;
	// Max temperature
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Temperature")
	float MaxTemperature;
	// Min temperature
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Temperature")
	float MinTemperature;

	// Is Rain Active
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain")
	uint8 bRain:1;

	class UAudioComponent* AudioComponent;
	class UParticleSystemComponent* ParticleComponent;

	// Current Season
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Season")
	TEnumAsByte<ESeasonType> CurrentSeason;

	// Duration of Season in minutes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Season")
	float SeasonDuration;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetRain(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void SetSeason(TEnumAsByte<ESeasonType> NewSeason);

	UFUNCTION(BlueprintCallable)
	void UpdateMaterialParameters();
};
