// Fill out your copyright notice in the Description page of Project Settings.


#include "Climate/WeatherComponent.h"

#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/KismetMaterialLibrary.h"

// Sets default values for this component's properties
UWeatherComponent::UWeatherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	CurrentSeason = Spring;

	RainIntensity = 0.0f;
}


// Called when the game starts
void UWeatherComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeatherComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RainIntensity = FMath::FInterpConstantTo(RainIntensity, float(bRain), DeltaTime, 0.1f);
	SnowIntensity = FMath::FInterpConstantTo(SnowIntensity, float(CurrentSeason == Winter),
		DeltaTime, 0.1f);
	AutumnIntensity = FMath::FInterpConstantTo(AutumnIntensity, float(CurrentSeason == Autumn),
		DeltaTime, 0.1f);
	
	// Rain sound and particle
	if(IsValid(AudioComponent))
	{
		AudioComponent->SetVolumeMultiplier(RainIntensity);
		if(!bRain && FMath::IsNearlyZero(RainIntensity))
		{
			AudioComponent->DestroyComponent();
		}
	}
	if(IsValid(ParticleComponent))
	{
		ParticleComponent->SetFloatParameter(TEXT("Intensity"), RainIntensity);
		if(!bRain && FMath::IsNearlyZero(RainIntensity))
		{
			ParticleComponent->DestroyComponent();
		}
		float ParticleParameter;
		ParticleComponent->GetFloatParameter(TEXT("Intensity"), ParticleParameter);
	}
	UpdateMaterialParameters();
}

void UWeatherComponent::SetRain(bool bEnabled)
{
	bRain = bEnabled;
	if(bRain)
	{
		USceneComponent* PlayerComponent = UGameplayStatics::GetPlayerCharacter(GetWorld(),
			0)->GetRootComponent();

		if(CurrentSeason != Winter)
		{
			AudioComponent = UGameplayStatics::SpawnSoundAttached(RainSound, PlayerComponent, FName(),
				FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
				true, RainIntensity, 1.0f, 0.0f,
				nullptr, nullptr, false);
			if(ParticleComponent)
			{
				ParticleComponent->DestroyComponent();
			}
			ParticleComponent = UGameplayStatics::SpawnEmitterAttached(RainParticle, PlayerComponent, FName(),
				FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
				true, EPSCPoolMethod::None, true);
			
		}
		else
		{
			if(AudioComponent)
			{
				AudioComponent->DestroyComponent();
			}
			if(ParticleComponent)
			{
				ParticleComponent->DestroyComponent();
			}
			ParticleComponent = UGameplayStatics::SpawnEmitterAttached(SnowParticle, PlayerComponent, FName(),
				FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
				true, EPSCPoolMethod::None, true);
		}
	}
}

void UWeatherComponent::SetSeason(TEnumAsByte<ESeasonType> NewSeason)
{
	CurrentSeason = NewSeason;
	SetRain(bRain);
}

void UWeatherComponent::UpdateMaterialParameters()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Collection,
		TEXT("RainIntensity"), RainIntensity);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Collection,
		TEXT("WindIntensity"), WindIntensity);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Collection,
		TEXT("SnowSharpness"), SnowIntensity);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Collection,
		TEXT("SnowBias"), SnowIntensity);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), Collection,
		TEXT("AutumnIntensity"), AutumnIntensity);
}
