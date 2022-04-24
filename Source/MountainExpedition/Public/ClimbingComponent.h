// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "ClimbingComponent.generated.h"

UENUM()
enum ECLimbingMode
{
    Climbing_None,
    Climbing_Wall,
    Climbing_Ledge,
    Climbing_Hang
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, config=Game )
class MOUNTAINEXPEDITION_API UClimbingComponent : public UActorComponent
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceType = EDrawDebugTrace::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta = (AllowPrivateAccess = "true"))
	class UPlayerAnimInstance* CharacterAnimInst;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta = (AllowPrivateAccess = "true"))
	float CharacterCapsuleHalfHeight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta = (AllowPrivateAccess = "true"))
	float CharacterCapsuleRadius;

	bool bDoMove;

public:	
	// Sets default values for this component's properties
	UClimbingComponent();

	// Constants
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector HangOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing (General Settings)")
		float MaxClimbingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing (General Settings)")
		float MaxFreeClimbingSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector2D MoveAxis;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClimbingTime")
		float ClimbingTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float JumpPrecision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 bJumpPrecisionIncrease : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsClimbing:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsHanging:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsJumping:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsJumpingRight:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsJumpingLeft:1;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsJumpingUp:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsTurnedBack:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanJump:1;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanClimb : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing State")
		uint8 bIsExitLedge : 1;
	

	// ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanMoveRight:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanMoveLeft:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanTurnRight:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanTurnLeft:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanJumpRight:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanJumpLeft:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing Action")
		uint8 bCanJumpUp:1;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector TargetRelativeLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FRotator TargetRelativeRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector WallLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector WallNormal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector LedgeLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector LedgeNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class ACableActor* RopeActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInterface* RopeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ADecalActor* DecalActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterialInterface* DecalMaterial;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Tracers
	UFUNCTION(BlueprintCallable)
		bool IsHanging();
	UFUNCTION(BlueprintCallable)
		bool SideTracer(float AxisValue, FHitResult &OutHit);

	UFUNCTION(BlueprintCallable)
		bool CanReachLedge(FVector Location);

	UFUNCTION(BlueprintCallable)
		bool TraceForWalls(FVector Start, FVector End, float SphereRadius, FVector& Location, FVector& Normal);
	UFUNCTION(BlueprintCallable)
		bool TraceForLedges(FVector StartLocation, FRotator StartRotation, FVector& Location, FVector& Normal);

	UFUNCTION(BlueprintCallable)
		bool TraceFarLedge(FVector StartLocation, FRotator StartRotation, FVector& Location, FRotator& Rotation);

	// Actions
	UFUNCTION(BlueprintCallable)
		void GrabLedge(int32 State, FVector TargetLocation, FRotator TargetRotation);
	UFUNCTION(BlueprintCallable)
		void ExitLedge();

	UFUNCTION(BlueprintCallable)
		void ClimbOnLedge(int32 State);
	UFUNCTION(BlueprintCallable)
		void JumpUpOnLedge(int32 State);

	// Movement
	UFUNCTION(BlueprintCallable)
		void MoveRight(float AxisValue);
	UFUNCTION(BlueprintCallable)
		void MoveForward(float AxisValue);
	UFUNCTION(BlueprintCallable)
		void Jump();
	UFUNCTION(BlueprintCallable)
		void StopJumping();

	UFUNCTION(BlueprintCallable)
		void AddClimbingTime(float Value);
		
	UFUNCTION(BlueprintCallable)
		bool CanMoveRight();

	// Get info about owner
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetActorLocation();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FRotator GetActorRotation();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetActorHangLocation();
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetActorEyeLocation();

	// Rope
	UFUNCTION(BlueprintCallable)
		void HookRope();
	UFUNCTION(BlueprintCallable)
		void UnhookRope();
};
