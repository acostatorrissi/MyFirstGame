// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYFIRSTGAME_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	void set_bar_value_percent(float const value);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* progress_value = nullptr;

	int32 const maxProgress = 5;
};
