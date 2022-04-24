// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* CharacterOwner;

public:
	UPlayerAnimInstance();

	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
		bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb State")
		bool bIsClimbing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb State")
		bool bIsHanging;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb State")
		bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Climb State")
		bool bCanJump;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bMoveRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bMoveLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bCanMoveRight; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bCanMoveLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bJumpRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bJumpLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bJumpUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climb State")
		bool bTurnBack;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Cheer();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ClimbOnLedge();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void JumpUpOnLedge();

	UFUNCTION(BlueprintCallable)
		void UpdateHandsLocation();

	// Character Bone Transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Pose")
		FRotator HeadRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Pose")
		FVector SpineOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Pose")
		float JumpingAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Pose")
		FVector LeftHandLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Pose")
		FVector RightHandLocation;
	
};
