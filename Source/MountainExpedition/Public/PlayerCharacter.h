// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCharacterParts
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName Username = FName(TEXT("Krzysztof"));
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Headwear = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Torso = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Legs = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Hands = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Boots = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* Backpack = nullptr;
};

UCLASS(config=Game)
class MOUNTAINEXPEDITION_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ClimbingSystem", meta = (AllowPrivateAccess = "true"))
	class UClimbingComponent* ClimbingComponent;

	/** Character Body Parts */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Headwear;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Torso;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Legs;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Hands;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Boots;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Backpack;

private:
	// Construction Script
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/* Player Character Params */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Parameters")
		float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Parameters")
		float Thirst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Parameters")
		float Hunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Parameters")
		float Temperature;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
		bool IsLookBehind;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
		bool IsRun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsDead:1;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
		void UseItem(class UItem* Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* SoundtrackSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* GameOverSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAudioComponent* SoundtrackAudio;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Player Input Events */
	// Handles input for moving froward and backward.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MoveForward(float AxisValue);

	// Handles input for moving right and left
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MoveRight(float AxisValue);

	virtual void Jump();
	virtual void StopJumping();

	// Handles input for Toggle Run
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Run();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StopRunning();

	// Handles input for Toggle View
	UFUNCTION()
	void ToggleView();

	UFUNCTION()
		void LookBehind();
	UFUNCTION()
		void LookForward();

	UFUNCTION(BlueprintCallable, Category = Default)
		void UpdateCharacterBody(FCharacterParts CharacterParts);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Cloth")
		TArray<class USkeletalMesh*> HeadwearMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Cloth")
		TArray<class USkeletalMesh*> TorsoMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Cloth")
		TArray<class USkeletalMesh*> LegsMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Cloth")
		TArray<class USkeletalMesh*> HandsMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Cloth")
		TArray<class USkeletalMesh*> BootsMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Cloth")
		TArray<class USkeletalMesh*> BackpackMeshes;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
