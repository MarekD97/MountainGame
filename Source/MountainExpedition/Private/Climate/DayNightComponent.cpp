// Fill out your copyright notice in the Description page of Project Settings.


#include "Climate/DayNightComponent.h"

#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"

// Sets default values for this component's properties
UDayNightComponent::UDayNightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentTime = 0.0f;
	DayNightLength = 20.0f;
}


// Called when the game starts
void UDayNightComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UDayNightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update Time
	CurrentTime += DeltaTime;
	
	// New Sun Angle
	float SunAngle = CurrentTime / (DayNightLength * 60.0f) * 360.0f - 180.0f;
	
	if(SunLight && SkyLight)
	{
		SunLight->SetActorRotation(FRotator(SunAngle, 0.0f, 0.0f));
		SunAngle = SunLight->GetActorRotation().Pitch;

		const float SunIntensity = FMath::GetMappedRangeValueClamped(FVector2D(15.0f, -30.0f),
		FVector2D(0.1f, 2.75f), SunAngle);
		const float SkyIntensity = FMath::GetMappedRangeValueClamped(FVector2D(15.0f, -30.0f),
			FVector2D(0.25f, 1.0f), SunAngle);
		SunLight->GetLightComponent()->SetIntensity(SunIntensity);
		SkyLight->GetLightComponent()->SetIntensity(SkyIntensity);
		SkyLight->GetLightComponent()->RecaptureSky();
	}
}

void UDayNightComponent::GetWorldTime(int& Hours, int& Minutes)
{
    // One game hour in seconds
    const float HourInGame = DayNightLength*60.0f / 24.0f;
	// One game minute in seconds
    const float MinuteInGame = HourInGame / 60.0f;

	Hours =  FMath::RoundToZero(FMath::Fmod(CurrentTime/HourInGame + 6.0f, 24.0f));
	Minutes = FMath::RoundToZero(FMath::Fmod(CurrentTime/MinuteInGame, 60.0f));
}

void UDayNightComponent::AddWorldTime(int Hours)
{
	const float NewTime = float(Hours)*60.0f*DayNightLength/24.0f;
	CurrentTime += NewTime;
}

