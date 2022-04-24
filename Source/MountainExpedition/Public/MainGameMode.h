#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8 {
	MainMenu = 0 UMETA(DisplayName = "MainMenu"),
	PauseMenu = 1 UMETA(DisplayName = "PauseMenu"),
	Gameplay = 2 UMETA(DisplayName = "Gameplay")
};

UCLASS(minimalapi)
class AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameMode CurrentGameMode;

	UFUNCTION(BlueprintCallable)
	void SetGameMode(EGameMode NewGameMode);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartNewGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadGame(const FString& SlotName, const int32 UserIndex);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPauseGame(bool bPause);

};
