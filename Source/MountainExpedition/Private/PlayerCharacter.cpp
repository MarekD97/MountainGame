// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Inventory/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PlayerAnimInstance.h"
#include "Inventory/Item.h"
#include "ClimbingComponent.h"
#include "MainPlayerController.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(32.0f, 94.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 240.0f;		// The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset.Y = 40.0f;
	CameraBoom->SocketOffset.Z = 40.0f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagMaxDistance = 60.0f;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;								// Camera does not rotate relative to arm

	/*SphereLedgeCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereLedgeCollision"));
	SphereLedgeCollision->SetupAttachment(RootComponent);
	SphereLedgeCollision->InitSphereRadius(94.0f * 2);
	SphereLedgeCollision->ShapeColor = FColor(FColor::Cyan);*/

	// Inventory Component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Climbing System
	ClimbingComponent = CreateDefaultSubobject<UClimbingComponent>(TEXT("ClimbingComponent"));

	// Character Body Parts
	Headwear = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Headwear"));
	Headwear->SetupAttachment(GetMesh());
	
	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(GetMesh());
	
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());
	
	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Hands->SetupAttachment(GetMesh());
	
	Boots = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
	Boots->SetupAttachment(GetMesh());

	Backpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Backpack"));
	Backpack->SetupAttachment(GetMesh());

	bIsDead = false;
	
	// Player Needs
	Energy = 1.0f;
	Thirst = 0.2f;
	Hunger = 0.2f;
	Temperature = 0.2f;
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Headwear->SetMasterPoseComponent(GetMesh(), false);
	Torso->SetMasterPoseComponent(GetMesh(), false);
	Legs->SetMasterPoseComponent(GetMesh(), false);
	Hands->SetMasterPoseComponent(GetMesh(), false);
	Boots->SetMasterPoseComponent(GetMesh(), false);
	Backpack->SetMasterPoseComponent(GetMesh(), false);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Camera
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAction("ToggleRun", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("ToggleRun", IE_Released, this, &APlayerCharacter::StopRunning);

	PlayerInputComponent->BindAction("ToggleView", IE_Pressed, this, &APlayerCharacter::ToggleView);
	PlayerInputComponent->BindAction("LookBehind", IE_Pressed, this, &APlayerCharacter::LookBehind);
	PlayerInputComponent->BindAction("LookBehind", IE_Released, this, &APlayerCharacter::LookForward);

	//PlayerInputComponent->BindAction("TurnBack", IE_Released, this, &APlayerCharacter::TurnBackLedge);
	//PlayerInputComponent->BindAction("TurnForward", IE_Released, this, &APlayerCharacter::TurnForwardLedge);
}

void APlayerCharacter::UseItem(UItem* Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); // BP Event
	}
}

void APlayerCharacter::Die_Implementation()
{
	if (bIsDead)
	{
		return;
	}
	bIsDead = true;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::SimulationUpatesComponentTransform;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DisableInput(Cast<APlayerController>(GetController()));
	if (SoundtrackAudio)
	{
		SoundtrackAudio->DestroyComponent();
	}
	Cast<AMainPlayerController>(GetController())->Die();
	UGameplayStatics::PlaySound2D(GetWorld(), GameOverSound);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SoundtrackAudio = UGameplayStatics::SpawnSound2D(GetWorld(), SoundtrackSound);
}

void APlayerCharacter::MoveForward_Implementation(float AxisValue)
{
	if (Controller == NULL) return;
	switch (GetCharacterMovement()->MovementMode)
	{
		case EMovementMode::MOVE_Walking:
		case EMovementMode::MOVE_Falling:
		{
			// Find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0, Rotation.Yaw, 0);
			if (IsLookBehind)
			{
				YawRotation.Yaw += 180.0f;
			}

			// Get right vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			// Add movement in that rotation

			AddMovementInput(Direction, AxisValue);
			break;
		}
		case EMovementMode::MOVE_Flying:
		{
			ClimbingComponent->MoveForward(AxisValue);
		}
	}
}

void APlayerCharacter::MoveRight_Implementation(float AxisValue)
{
	if (Controller == NULL) return;
	switch (GetCharacterMovement()->MovementMode)
	{
		case EMovementMode::MOVE_Walking:
		case EMovementMode::MOVE_Falling:
		{
			// Find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0, Rotation.Yaw, 0);
			if (IsLookBehind)
			{
				YawRotation.Yaw += 180.0f;
			}

			// Get right vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// Add movement in that rotation

			AddMovementInput(Direction, AxisValue);
			break;
		}
		case EMovementMode::MOVE_Flying:
		{
			ClimbingComponent->MoveRight(AxisValue);
		}
	}
}

void APlayerCharacter::Jump()
{
	switch (GetCharacterMovement()->MovementMode)
	{
		case EMovementMode::MOVE_Walking:
		case EMovementMode::MOVE_Falling:
		{
			Super::Jump();
			break;
		}
		case EMovementMode::MOVE_Flying:
		{
			ClimbingComponent->Jump();
			break;
		}
	}
}

void APlayerCharacter::StopJumping()
{
	switch (GetCharacterMovement()->MovementMode)
	{
	case EMovementMode::MOVE_Walking:
	case EMovementMode::MOVE_Falling:
	{
		Super::StopJumping();
		break;
	}
	case EMovementMode::MOVE_Flying:
	{
		ClimbingComponent->StopJumping();
		break;
	}
	}
}

void APlayerCharacter::ToggleView()
{
	return;
	if (CameraBoom->TargetArmLength == 240.0f)
	{
		CameraBoom->bEnableCameraLag = false;
		CameraBoom->bEnableCameraRotationLag = false;
		/*CameraBoom->TargetArmLength = 0.0f;
		CameraBoom->SocketOffset = FVector(10.0f, 0.0f, 56.0f);*/
		this->bUseControllerRotationYaw = true;
	}
	else
	{
		CameraBoom->bEnableCameraLag = true;
		CameraBoom->bEnableCameraRotationLag = true;
		CameraBoom->TargetArmLength = 240.0f;
		CameraBoom->SocketOffset = FVector(0.0f, 40.0f, 40.0f);
		this->bUseControllerRotationYaw = false;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("TargetArmLength: %d"), CameraBoom->TargetArmLength));
}

void APlayerCharacter::LookBehind()
{
	IsLookBehind = true;
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += 180.0f;
	GetController()->SetControlRotation(NewRotation);
}

void APlayerCharacter::LookForward()
{
	IsLookBehind = false;
	FRotator NewRotation = GetActorRotation();
	GetController()->SetControlRotation(NewRotation);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Character max walk speed
	GetCharacterMovement()->MaxWalkSpeed = (200.0f * Energy + 200.0f*(int)IsRun) + 200.0f;

	// Update Player Needs
	Energy += DeltaTime* (Hunger+Thirst-1.0f)/240.0f;
	if (ClimbingComponent->bIsClimbing)
	{
		Energy -= GetVelocity().Size()/600.0f * DeltaTime / 60.0f;
	}
	else
	{
		Energy -= GetVelocity().Size()/600.0f * DeltaTime / 240.0f;
	}
	if (Energy > 1.0f)
	{
		Energy = 1.0f;
	}

	Hunger -= DeltaTime / 600.0f;
	Thirst -= DeltaTime / 600.0f;
	if (Hunger > 1.0f)
	{
		Hunger = 1.0f;
	}
	if (Thirst > 1.0f)
	{
		Thirst = 1.0f;
	}
	if (Energy <= 0.0f || Hunger <= 0.0f || Thirst <= 0.0f)
	{
		Die();
	}

	// Falling
	if(GetCharacterMovement()->IsFalling() && GetVelocity().Z < -1400.0f)
	{
		Die();
	}
}

void APlayerCharacter::Run_Implementation()
{
	ClimbingComponent->ExitLedge();
	IsRun = Energy > 0.25f;
}

void APlayerCharacter::StopRunning_Implementation()
{
	IsRun = false;
}

void APlayerCharacter::UpdateCharacterBody(FCharacterParts CharacterParts)
{
	Headwear->SetSkeletalMesh(CharacterParts.Headwear, true);
	Torso->SetSkeletalMesh(CharacterParts.Torso, true);
	Legs->SetSkeletalMesh(CharacterParts.Legs, true);
	Hands->SetSkeletalMesh(CharacterParts.Hands, true);
	Boots->SetSkeletalMesh(CharacterParts.Boots, true);
	Backpack->SetSkeletalMesh(CharacterParts.Backpack, true);
}