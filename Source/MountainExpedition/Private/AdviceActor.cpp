// Fill out your copyright notice in the Description page of Project Settings.


#include "AdviceActor.h"
#include "PaperSpriteComponent.h"
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "Sound/SoundBase.h"


AAdviceActor::AAdviceActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	Sprite->SetupAttachment(SphereCollision);
	Sprite->AddRelativeLocation(FVector(0.0f, 0.0f, OverlapRadius/2.0f), false);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FText AAdviceActor::GetKeyDisplayName(FName Name)
{
	const UInputSettings* InputSettings = UInputSettings::GetInputSettings();
	TArray<FInputActionKeyMapping> OutMappings;
	
	InputSettings->GetActionMappingByName(ActionName, OutMappings);
	if(OutMappings.Num() > 0)
	{
		for (const FInputActionKeyMapping OutMapping : OutMappings)
		{
			const FKey Key = OutMapping.Key;
			if(Key.IsMouseButton()|| !Key.IsGamepadKey())
			{
				// Keyboard or Mouse button
				return Key.GetDisplayName();
			}
		}
	}
	else
	{
		TArray<FInputAxisKeyMapping> OutAxisMappings;
		InputSettings->GetAxisMappingByName(ActionName, OutAxisMappings);
		if(OutAxisMappings.Num() > 0)
		{
			TArray<FKey> AxisKeys;
			for (const FInputAxisKeyMapping OutAxisMapping : OutAxisMappings)
			{
				const FKey Key = OutAxisMapping.Key;
				if(Key.IsMouseButton() || !Key.IsGamepadKey())
				{
					AxisKeys.Add(Key);
				}
			}
			FFormatOrderedArguments Args;
			Args.Add(AxisKeys[0].GetDisplayName());
			Args.Add(AxisKeys[1].GetDisplayName());
			return FText::Format(NSLOCTEXT("Keys", "Keys", "{0} {1}"), Args);
		}
	}
	return FText::GetEmpty();
}

void AAdviceActor::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	
}

void AAdviceActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(PlayerRef)
	{
		FMinimalViewInfo DesiredView;
		PlayerRef->GetFollowCamera()->GetCameraView(DeltaSeconds, DesiredView);
		const FRotator NewRotation = FRotator(DesiredView.Rotation.Roll, DesiredView.Rotation.Yaw - 90.0f, -DesiredView.Rotation.Pitch);
		Sprite->SetWorldRotation(NewRotation);
	}
}

void AAdviceActor::BeginInteract_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	
	const FText DisplayName = GetKeyDisplayName(ActionName);
	FFormatNamedArguments Args;
	Args.Add(TEXT("ActionName"), DisplayName);
	const FText DisplayMessageText = FText::Format(MessageText, Args);
	FOutputDeviceNull ar;
	GetWorld()->GetFirstPlayerController()->GetHUD()->CallFunctionByNameWithArguments(TEXT("ShowMessage"), ar, NULL, false);
	
}
