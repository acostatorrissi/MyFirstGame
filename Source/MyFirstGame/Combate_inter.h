// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Combate_inter.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class MYFIRSTGAME_API UCombate_inter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYFIRSTGAME_API ICombate_inter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int melee_attack();
	
};
