// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFirstGameGameMode.h"
#include "MyFirstGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

AMyFirstGameGameMode::AMyFirstGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMyFirstGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Set the score to beat
	AMyFirstGameCharacter* MyCharacter = Cast<AMyFirstGameCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)); //useless

	if(MyCharacter)
	{
		RocksToWin = 5.0f; //check this
	}

	if(HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if(CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
	
}

float AMyFirstGameGameMode::GetRocksToWin() const
{
	return RocksToWin;
}


