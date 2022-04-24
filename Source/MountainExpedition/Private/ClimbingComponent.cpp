// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"

#include "DrawDebugHelpers.h"
#include "PlayerCharacter.h"
#include "PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "CableComponent.h"
#include "CableActor.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/SpringArmComponent.h"

UClimbingComponent::UClimbingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bDoMove = false;

	MaxClimbingSpeed = 60.0f;
	MaxFreeClimbingSpeed = 45.0f;

	bCanClimb = false;
	bIsExitLedge = false;
}


// Called when the game starts
void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<APlayerCharacter>(GetOwner());
	if (Character)
	{
		CharacterCapsuleHalfHeight = Character->GetSimpleCollisionHalfHeight();
		CharacterCapsuleRadius = Character->GetSimpleCollisionRadius();
		CharacterAnimInst = Cast<UPlayerAnimInstance>(Character->GetMesh()->GetAnimInstance());

		ClimbingTime = Character->Energy / 2.0f + 0.5f;

		// Hang offset
		HangOffset = FVector(-CharacterCapsuleRadius, 0.0f, -CharacterCapsuleHalfHeight);

	}
	
}

// Called every frame
void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Detect wall in front of character
	if (TraceForWalls(GetActorEyeLocation(), GetActorEyeLocation() + GetActorRotation().RotateVector(FVector(150.0f, 0.0f, 0.0f)), 20.0f, WallLocation, WallNormal))
	{
		// Detect ledge over the WallLocation
		if (TraceForLedges(WallLocation + (-WallNormal).Rotation().RotateVector(FVector(0.0f, 0.0f, 150.0f)), (-WallNormal).Rotation(), LedgeLocation, LedgeNormal))
		{
			if (!bIsClimbing && !bIsExitLedge && CanReachLedge(LedgeLocation) && FVector::DotProduct(LedgeNormal, FVector::UpVector) >= -0.5f)
			{
				GrabLedge(0, LedgeLocation + (-LedgeNormal).Rotation().RotateVector(HangOffset), (-LedgeNormal).Rotation());
			}
		}
	}

	if (bIsClimbing)
	{
		bIsHanging = IsHanging();
	}

	if (!bCanJump && bCanClimb && MoveAxis.Size() > 0)
	{
		bCanJump = true;
		JumpPrecision = FMath::RandRange(-1.0f, 1.0f);
		bJumpPrecisionIncrease = true;
	}

	// Sphere Tracer Far Ledge
	if (!bIsJumping && MoveAxis.Size() > 0.0f)
	{
		const float JumpDistance = 200.0f;
		const float SphereRadius = 100.0f;

		const FVector2D JumpAxis = MoveAxis.GetSafeNormal();

		const FVector Offset = FVector(0.0f, JumpDistance * JumpAxis.X, JumpDistance * JumpAxis.Y);
		const FVector Start = GetActorEyeLocation() + GetActorRotation().RotateVector(Offset + FVector(-100.0f, 0.0f, 0.0f));
		const FVector End = Start + GetActorRotation().RotateVector(FVector(150.0f, 0.0f, 0.0f));

		// Detect Wall
		if (TraceForWalls(Start, End, SphereRadius, WallLocation, WallNormal))
		{
			// Detect Ledge
			if (TraceForLedges(WallLocation + (-WallNormal).Rotation().RotateVector(FVector(0.0f, 0.0f, 150.0f)), (-WallNormal).Rotation(), LedgeLocation, LedgeNormal)
				&&	FVector::DotProduct(LedgeNormal, FVector::UpVector) >= -0.5f)
			{
				TargetRelativeLocation = LedgeLocation + (-LedgeNormal).Rotation().RotateVector(HangOffset);
				TargetRelativeRotation = (-LedgeNormal).Rotation();

				//JumpPrecision += DeltaTime * (bJumpPrecisionIncrease ? 1.0f : -1.0f) * 2.0f;
				JumpPrecision = UKismetMathLibrary::Lerp(JumpPrecision, bJumpPrecisionIncrease ? 2.0f : -2.0f, DeltaTime);
				if (JumpPrecision >= 1.0f || JumpPrecision <= -1.0f)
				{
					bJumpPrecisionIncrease = !bJumpPrecisionIncrease;
				}

				const FVector2D JumpOffset = FMath::RandPointInCircle(FMath::Pow(FMath::Abs(JumpPrecision), 2)*200.0f);
				TargetRelativeLocation += TargetRelativeRotation.RotateVector(FVector(0.0f, JumpOffset.X, JumpOffset.Y));

			}
			else
			{
				TargetRelativeLocation = FVector::ZeroVector;
				TargetRelativeRotation = FRotator::ZeroRotator;
				bCanJump = false;
			}

			if (!DecalActor)
			{
				if (bCanJump)
				{
					DecalActor = GetWorld()->SpawnActor<ADecalActor>(TargetRelativeLocation, WallNormal.Rotation());
					DecalActor->SetDecalMaterial(DecalMaterial);
				}
			}
			else
			{
				DecalActor->SetActorLocationAndRotation(WallLocation, WallNormal.Rotation());
			}
		}
		else
		{
			TargetRelativeLocation = FVector::ZeroVector;
			TargetRelativeRotation = FRotator::ZeroRotator;
			bCanJump = false;

			if (DecalActor)
			{
				DecalActor->Destroy();
				DecalActor = nullptr;
			}
		}

	}
	else
	{
		JumpPrecision = 0.0f;

		if (DecalActor)
		{
			DecalActor->Destroy();
			DecalActor = nullptr;
		}
	}

	// Update ClimbingTime
	if (bIsClimbing)
	{
		if (ClimbingTime == 0.0f)
		{
			ClimbingTime = Character->Energy / 2.0f + 0.5f;
		}
		else
		{
			ClimbingTime -= DeltaTime * (1.0f + bIsHanging)/50.0f;
			if (ClimbingTime <= 0.0f)
			{
				ClimbingTime = 0.0f;
				ExitLedge();
			}
		}
	}
	else
	{
		ClimbingTime = 0.0f;
	}

	// Camera Offset
	if (Character && Character->CameraBoom)
	{
		const FVector NewOffset = bIsClimbing ? FVector(-240.0f, 0.0f, 160.0f) : FVector(0.0f, 40.0f, 40.0f);

		Character->CameraBoom->SocketOffset = FMath::Lerp(Character->CameraBoom->SocketOffset, NewOffset, DeltaTime);
	}

}

bool UClimbingComponent::IsHanging()
{
	FHitResult OutHit;

	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + GetActorRotation().RotateVector(FVector(50.0f, 0.0f, 0.0f));

	if (UKismetSystemLibrary::SphereTraceSingle(this, TraceStart, TraceEnd, 20.0f, ETraceTypeQuery::TraceTypeQuery1, false,
		TArray<AActor*>(), DrawDebugTraceType, OutHit, true))
	{
		return false;
	}
	return true;
}

bool UClimbingComponent::SideTracer(float AxisValue, FHitResult& OutHit)
{
	if (Character && !FMath::IsNearlyZero(AxisValue))
	{

		const FVector Offset = FVector(0.0f, 32.0f * FMath::Sign(AxisValue), Character->GetDefaultHalfHeight() - 20.0f);
		const FVector TraceStart = Character->GetActorLocation() + Character->GetActorRotation().RotateVector(Offset);
		const FVector TraceEnd = TraceStart + Character->GetActorUpVector()*(-2.0f*Character->GetSimpleCollisionHalfHeight()+20.0f);

		bool IsHit = UKismetSystemLibrary::SphereTraceSingle(this,
			TraceStart, TraceEnd, 20.0f, 
			ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), DrawDebugTraceType,
			OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

		if (!IsHit && AxisValue > 0.0f)
		{
			bCanMoveRight = true;
			bCanMoveLeft = false;
		}
		else if (!IsHit && AxisValue < 0.0f)
		{
			bCanMoveRight = false;
			bCanMoveLeft = true;
		}
		else {
			bCanMoveRight = bCanMoveLeft = false;
		}
		return IsHit;
	}
	return false;
}

bool UClimbingComponent::CanReachLedge(FVector Location)
{
	if (Character)
	{
		float SocketDistance = (Character->GetMesh()->GetSocketLocation(FName(TEXT("PelvisSocket"))) - Location).Size();
		if (SocketDistance <= 100.0f)
		{
			return true;
		}
	}
	return false;
}

bool UClimbingComponent::TraceForWalls(FVector Start, FVector End, float SphereRadius, FVector& Location, FVector& Normal)
{
	FHitResult OutHit;

	if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, SphereRadius, ETraceTypeQuery::TraceTypeQuery3, false, 
		TArray<AActor*>(), DrawDebugTraceType, OutHit, true))
	{
		Location = OutHit.ImpactPoint;
		Normal = OutHit.ImpactNormal;
		return true;
	}

	Location = Normal = FVector::ZeroVector;
	return false;
}

bool UClimbingComponent::TraceForLedges(FVector StartLocation, FRotator StartRotation, FVector& Location, FVector& Normal)
{
	FVector Start = StartLocation + StartRotation.RotateVector(FVector(20.0f, 0.0f, 0.0f));
	FVector End = Start + StartRotation.RotateVector(FVector(0.0f, 0.0f, -150.0f));

	FHitResult OutHit;

	if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, 5.0f, ETraceTypeQuery::TraceTypeQuery3, false,
		TArray<AActor*>(), DrawDebugTraceType, OutHit, true))
	{
		if (OutHit.Location == Start)
		{
			return false;
		}
		Location.Z = OutHit.ImpactPoint.Z;

		Start = OutHit.ImpactPoint + StartRotation.RotateVector(FVector(-100.0f, 0.0f, -20.0f));
		End = Start + StartRotation.RotateVector(FVector(150.0f, 0.0f, 0.0f));

		if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, 5.0f, ETraceTypeQuery::TraceTypeQuery3, false,
			TArray<AActor*>(), DrawDebugTraceType, OutHit, true))
		{
			Location.X = OutHit.ImpactPoint.X;
			Location.Y = OutHit.ImpactPoint.Y;
			Normal = OutHit.ImpactNormal;

			return true;
		}
	}
	Location = Normal = FVector::ZeroVector;
	return false;
}

bool UClimbingComponent::TraceFarLedge(FVector StartLocation, FRotator StartRotation, FVector& Location, FRotator& Rotation)
{
	const FVector Start = StartLocation + StartRotation.RotateVector(FVector(-100.0f, 0.0f, 0.0f));
	const FVector End = Start + StartRotation.RotateVector(FVector(150.0f, 0.0f, 0.0f));

	FHitResult OutHit;

	if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, 100.0f, ETraceTypeQuery::TraceTypeQuery3, false,
		TArray<AActor*>(), DrawDebugTraceType, OutHit, true))
	{
		Location = OutHit.ImpactPoint;
		Rotation = (OutHit.ImpactNormal * (-1)).Rotation();
		return true;
	}

	return false;
}

void UClimbingComponent::GrabLedge(int32 State, FVector TargetLocation, FRotator TargetRotation)
{
	switch (State)
	{
	case 0:
		{
			if (Character && !bIsExitLedge)
			{
				Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				Character->GetCharacterMovement()->bOrientRotationToMovement = false;
				bIsClimbing = true;

				TargetRelativeLocation = TargetLocation;
				TargetRelativeRotation = TargetRotation;

				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				LatentInfo.ExecutionFunction = "GrabLedge";
				LatentInfo.Linkage = 1;

				UKismetSystemLibrary::MoveComponentTo(Character->GetRootComponent(), TargetRelativeLocation, TargetRelativeRotation, false, false,
					0.13f, false, EMoveComponentAction::Move, LatentInfo);
			}
			break;
		}
	case 1:
		{
			Character->GetCharacterMovement()->StopMovementImmediately();
		}
	default:
		break;
	}
}

void UClimbingComponent::MoveRight(float AxisValue)
{
	MoveAxis.X = AxisValue;
	if (Character && bIsClimbing && !bIsJumping && !bCanJump)
	{
		FHitResult OutHit;

		if (SideTracer(AxisValue, OutHit))
		{
			Character->GetCharacterMovement()->StopMovementImmediately();
		}
		else
		{
			const FVector Offset = GetActorLocation() + GetActorRotation().RotateVector(FVector(0.0f, AxisValue * 40.0f, 40.0f) - HangOffset);

			const bool bNearLedge = TraceForLedges(Offset, GetActorRotation(), LedgeLocation, LedgeNormal);
			const bool bCanReach = (GetActorLocation() - LedgeLocation).Size() < 120.0f;

			if (bNearLedge && bCanReach)
			{
				const float ClimbingSpeed = !bIsHanging ? MaxClimbingSpeed : MaxFreeClimbingSpeed;
				Character->AddMovementInput(Character->GetActorRightVector(), ClimbingSpeed * AxisValue / 100.0f, false);

				FRotator NewRotation = UKismetMathLibrary::RInterpTo(Character->GetActorRotation(), (-LedgeNormal).Rotation(), GetWorld()->GetDeltaSeconds(), 2.0f);
				Character->SetActorRotation(NewRotation);
				Character->GetCharacterMovement()->StopMovementImmediately();

			}
			else
			{
				Character->GetCharacterMovement()->StopMovementImmediately();
			}
		}
	}
	else
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
	}
}

void UClimbingComponent::MoveForward(float AxisValue)
{
	MoveAxis.Y = AxisValue;
}

void UClimbingComponent::Jump()
{
	if (MoveAxis.Size() > 0.0f)
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
	}
	bCanClimb = true;

}

void UClimbingComponent::StopJumping()
{
	if (bCanJump && !bIsJumping)
	{
		JumpUpOnLedge(0);
	}
	else if (bCanClimb && MoveAxis.Size() == 0.0f && !bDoMove)
	{
		ClimbOnLedge(0);
	}
	bCanClimb = false;
	bCanJump = false;

}

void UClimbingComponent::AddClimbingTime(float Value)
{
	if (ClimbingTime < Character->Energy / 2.0f + 0.5f)
	{
		ClimbingTime += Value;
		if (ClimbingTime > Character->Energy / 2.0f + 0.5f)
		{
			ClimbingTime = Character->Energy / 2.0f + 0.5f;
		}
	}
}

bool UClimbingComponent::CanMoveRight()
{
	return false;
}

FVector UClimbingComponent::GetActorLocation()
{
	return GetOwner()->GetActorLocation();
}

FRotator UClimbingComponent::GetActorRotation()
{
	return GetOwner()->GetActorRotation();
}

FVector UClimbingComponent::GetActorHangLocation()
{
	const FVector Location = GetOwner()->GetActorLocation();
	const FVector Offset = GetOwner()->GetActorRotation().RotateVector(HangOffset);
	return Location + Offset;
}

FVector UClimbingComponent::GetActorEyeLocation()
{
	const FVector Location = GetOwner()->GetActorLocation();
	const float EyeHeight = Cast<ACharacter>(GetOwner())->BaseEyeHeight;
	const FVector Offset = GetOwner()->GetActorUpVector() * EyeHeight;
	return Location + Offset;
}

void UClimbingComponent::ExitLedge()
{
	if (Character && bIsClimbing)
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		bIsTurnedBack = false;
		FRotator NewRotation = FRotator(0.0f, Character->GetActorRotation().Yaw, 0.0f);
		Character->SetActorRotation(NewRotation);
		bIsClimbing = bIsHanging = bCanClimb = bCanJump = false;

		bIsExitLedge = true;
		FTimerHandle Handle;
		// Delay 1.0s
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]() {
			bIsExitLedge = false;

			if (RopeActor && Character->GetCharacterMovement()->MovementMode == MOVE_Falling)
			{
				if (FVector::Dist(GetActorLocation(), RopeActor->GetActorLocation()) < 800.0f)
				{
					Character->SetActorLocation(RopeActor->GetActorLocation());
				}
			}
			else
			{
				UnhookRope();
			}

		}, 1.0f, false, 1.0f);
	}
}

void UClimbingComponent::ClimbOnLedge(int32 State)
{
	switch (State)
	{
	case 0:
	{
		bDoMove = true;

		if(LedgeLocation != FVector::ZeroVector && LedgeNormal != FVector::ZeroVector)
		{
			TargetRelativeRotation = FRotator::ZeroRotator;
			TargetRelativeRotation.Yaw = (-LedgeNormal).Rotation().Yaw;
			TargetRelativeLocation = LedgeLocation + TargetRelativeRotation.RotateVector(FVector(16.0f, 0.0f, 96.0f));

			// Can Climb On Ledge
			const FVector Start = TargetRelativeLocation + TargetRelativeRotation.RotateVector(FVector(0.0f, 0.0f, 54.0f));
			const FVector End = TargetRelativeLocation + TargetRelativeRotation.RotateVector(FVector(0.0f, 0.0f, -54.0f));

			FHitResult OutHit;

			if (!UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 32.0f, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(),
				DrawDebugTraceType, OutHit, true))
			{
				CharacterAnimInst->ClimbOnLedge();
			}
			else
			{
				bDoMove = false;
			}
		}

		break;
	}
	case 1:
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "ClimbOnLedge";
		LatentInfo.Linkage = 2;

		// Move character to ledge
		UKismetSystemLibrary::MoveComponentTo(Character->GetRootComponent(), TargetRelativeLocation, TargetRelativeRotation, false, false,
			0.3f, false, EMoveComponentAction::Move, LatentInfo);
		break;
	}
	case 2:
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		bIsClimbing = bIsHanging = false;
		bDoMove = false;
		break;
	}
	default:
		break;
	}

}

void UClimbingComponent::JumpUpOnLedge(int32 State)
{
	switch (State)
	{
	case 0:
	{
		if (TargetRelativeLocation == FVector::ZeroVector || TargetRelativeRotation == FRotator::ZeroRotator)
		{
			return;
		}
		bIsJumping = true;
		if (!bDoMove)
		{
			bDoMove = true;

			if (MoveAxis.X > 0.0f)
			{
				bIsJumpingRight = true;
				bIsJumpingLeft = false;
				bIsJumpingUp = false;
			}
			else if (MoveAxis.X < 0.0f)
			{
				bIsJumpingRight = false;
				bIsJumpingLeft = true;
				bIsJumpingUp = false;
			}

			if (MoveAxis.Y > 0.0f)
			{
				bIsJumpingRight = false;
				bIsJumpingLeft = false;
				bIsJumpingUp = true;
			}

			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;
			LatentInfo.ExecutionFunction = "JumpUpOnLedge";
			LatentInfo.Linkage = 1;

			UKismetSystemLibrary::Delay(this, 0.2f, LatentInfo);
		}
		break;

	}
	case 1:
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = "JumpUpOnLedge";
		LatentInfo.Linkage = 2;

		UKismetSystemLibrary::MoveComponentTo(Character->GetRootComponent(), TargetRelativeLocation, TargetRelativeRotation, false, false,
				0.7f, false, EMoveComponentAction::Move, LatentInfo);
		break;
	}
	case 2:
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
		bIsJumping = bIsJumpingRight = bIsJumpingLeft = bIsJumpingUp = false;


		if (FVector::Distance(LedgeLocation, GetActorLocation() + GetActorRotation().RotateVector(-HangOffset)) > 50.0f)
		{
			ExitLedge();
		}
		else
		{
			GrabLedge(0, LedgeLocation + (-LedgeNormal).Rotation().RotateVector(HangOffset), (-LedgeNormal).Rotation());
		}
		bDoMove = false;
		break;
	}
	default:
		break;
	}

}

void UClimbingComponent::HookRope()
{
	if (bIsClimbing)
	{
		if (RopeActor)
		{
			RopeActor->CableComponent->SetAttachEndTo(nullptr, FName());
			RopeActor->CableComponent->EndLocation = GetActorLocation() - RopeActor->GetActorLocation();
			RopeActor->CableComponent->CableLength = FVector::Distance(GetActorLocation(), RopeActor->GetActorLocation());
			RopeActor = nullptr;
		}
		else
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Instigator = Cast<APawn>(GetOwner());
			RopeActor = GetWorld()->SpawnActor<ACableActor>(WallLocation, FRotator::ZeroRotator, SpawnInfo);
			RopeActor->CableComponent->EndLocation = FVector::ZeroVector;
			RopeActor->CableComponent->SetAttachEndTo(GetOwner(), FName(), FName(TEXT("PelvisSocket")));
			RopeActor->CableComponent->SetMaterial(0, RopeMaterial);
			RopeActor->CableComponent->CableLength = 500.0f;
			RopeActor->CableComponent->CableWidth = 3.0f;
			RopeActor->CableComponent->bEnableCollision = true;
			RopeActor->SetLifeSpan(120.0f);

		}
	}
}

void UClimbingComponent::UnhookRope()
{
	if (RopeActor)
	{
		RopeActor->CableComponent->SetAttachEndTo(nullptr, FName());
		RopeActor->CableComponent->bAttachEnd = false;
		RopeActor->CableComponent->EndLocation = GetActorLocation() - RopeActor->GetActorLocation();
		RopeActor->CableComponent->CableLength = FVector::Distance(GetActorLocation(), RopeActor->GetActorLocation());
		RopeActor = nullptr;
	}
}
