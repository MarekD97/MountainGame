#include "MainGameMode.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AMainGameMode::AMainGameMode() 
{
	// Set current gamemode
	SetGameMode(EGameMode::MainMenu);

	//GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameMode::SetGameMode(EGameMode NewGameMode)
{
	CurrentGameMode = NewGameMode;
}

void AMainGameMode::StartNewGame_Implementation()
{
	CurrentGameMode = EGameMode::Gameplay;
	
}

void AMainGameMode::LoadGame_Implementation(const FString& SlotName, const int32 UserIndex)
{
}

void AMainGameMode::SetPauseGame_Implementation(bool bPause)
{
	UGameplayStatics::SetGamePaused(this, bPause);
	

}
