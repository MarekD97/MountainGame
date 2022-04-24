// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerQuest.h"
#include "PlayerCharacter.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerQuest::APlayerQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	Cylinder->SetupAttachment(RootComponent);
	Cylinder->SetCollisionProfileName(TEXT("OverlapAll"));

	Sprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(Cylinder);

	Cylinder->OnComponentBeginOverlap.AddDynamic(this, &APlayerQuest::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void APlayerQuest::BeginPlay()
{
	Super::BeginPlay();
	SetActive(bEnabled);
}

// Called every frame
void APlayerQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerQuest::SetActive(bool bEnable)
{
	SetActorTickEnabled(bEnable);
	SetActorHiddenInGame(!bEnable);
	SetActorEnableCollision(bEnable);
	bEnabled = bEnable;
}

void APlayerQuest::OnComponentBeginOverlap
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComponent, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	const APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
	if (Character)
	{
		ShowQuest();
		if (EnterSound)
		{
			UGameplayStatics::SpawnSoundAttached(EnterSound, Cylinder);
		}
		if (NextQuest)
		{
			NextQuest->SetActive(true);
		}
		Destroy();
	}
}

