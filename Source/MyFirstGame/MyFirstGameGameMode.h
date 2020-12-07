// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyFirstGameGameMode.generated.h"

UCLASS(minimalapi)
class AMyFirstGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyFirstGameGameMode();

	// Returns rocks samples needed for the HUD
	UFUNCTION(BlueprintPure, Category="Rocks")
	float GetRocksToWin() const;

	virtual void BeginPlay() override;
	
protected:

	//The rock samples needed to win the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Rocks", Meta = (BlueprintProtected = "true"))
	float RocksToWin;

	// The Widget class to use for our HUD screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rocks", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// The instance of the HUD
	UPROPERTY()
	class UUserWidget* CurrentWidget;
};



