// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class MYFIRSTGAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Returns the WhereToSpawn subobject
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	// Finds a random point within the box component
	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetRandomPointInVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Box Component to specify where pickups should be spawned
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

	// The pickup to spawn
	UPROPERTY(EditAnywhere, Category ="Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	// Minimum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	float SpawnDelayRangeLow;
	
	// Maximum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;
private:

	// Handle spawning a new pickup
	void SpawnPickup();

	//The current spawn delay
	float SpawnDelay;
};
