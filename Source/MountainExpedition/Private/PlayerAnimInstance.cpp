// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ClimbingComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	bIsInAir = false;
	bIsClimbing = bIsHanging = bIsJumping = false;

	Speed = Direction = 0.0f;
	LeftHandLocation = FVector(30.0f, 30.0f, 192.0f);
	RightHandLocation = FVector(-30.0f, 30.0f, 192.0f);
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CharacterOwner = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (CharacterOwner)
	{
		// Movement Speed and Direction
		Speed = CharacterOwner->GetVelocity().Size();
		Direction = FVector::DotProduct(CharacterOwner->GetActorRightVector(), CharacterOwner->GetVelocity());
			
		bIsInAir = CharacterOwner->GetMovementComponent()->IsFalling();

		if (UClimbingComponent* ClimbingComponent = CharacterOwner->ClimbingComponent)
		{
			bIsClimbing = ClimbingComponent->bIsClimbing;
			bIsHanging = ClimbingComponent->bIsHanging;
			bIsJumping = ClimbingComponent->bIsJumping;
			bCanJump = ClimbingComponent->bCanJump;

			JumpingAlpha += (float(bCanJump) - JumpingAlpha) * DeltaTime;

			if (ClimbingComponent->bIsJumpingRight)
			{
				Direction = 1.0f;
			}
			else if (ClimbingComponent->bIsJumpingLeft)
			{
				Direction = -1.0f;
			} 
			else if (ClimbingComponent->bIsJumpingUp)
			{
				Speed = 1.0f;
				Direction = 0.0f;
			}
			HeadRotation = FRotator(0.0f, ClimbingComponent->MoveAxis.X * 60.0f, 0.0f);
			SpineOffset = FVector(ClimbingComponent->MoveAxis.X * 5.0f, 0.0f, 0.0f);

			if (bIsClimbing)
			{
				UpdateHandsLocation();
			}
		}
	}
}

void UPlayerAnimInstance::UpdateHandsLocation()
{
	const float HandSpacing = 32.0f;
	if (CharacterOwner)
	{
		for (uint8 Hand = 0; Hand < 2; Hand++)
		{
			const FVector CharacterLocation = CharacterOwner->GetActorLocation();
			const FRotator CharacterRotation = CharacterOwner->GetActorRotation();

			FVector Start = CharacterLocation + CharacterRotation.RotateVector(FVector(0.0f, Hand == 0 ? HandSpacing : -HandSpacing, 92.0f));
			FVector End = Start + CharacterRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));

			FHitResult OutHit;

			if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery3, false, TArray<AActor*>(),
				EDrawDebugTrace::None, OutHit, true))
			{
				if (Hand == 0)
				{
					RightHandLocation = OutHit.Location;
				}
				else
				{
					LeftHandLocation = OutHit.Location;
				}
			}
			else
			{
				Start = CharacterLocation + CharacterRotation.RotateVector(FVector(42.0f, Hand == 0 ? HandSpacing : -HandSpacing, 92.0f));
				End = Start + CharacterRotation.RotateVector(FVector(0.0f, Hand == 0 ? -100.0f : 100.0f, 0.0f));

				if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery3, false, TArray<AActor*>(),
					EDrawDebugTrace::None, OutHit, true))
				{
					if (Hand == 0)
					{
						RightHandLocation = OutHit.Location + CharacterRotation.RotateVector(FVector(-10.0f, 0.0f, 0.0f));
					}
					else
					{
						LeftHandLocation = OutHit.Location + CharacterRotation.RotateVector(FVector(-10.0f, 0.0f, 0.0f));
					}
				}
			}
		}
	}
}
