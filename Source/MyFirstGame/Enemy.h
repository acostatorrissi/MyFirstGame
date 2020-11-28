// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combate_inter.h"
#include "Animation/AnimMontage.h"
#include "Enemy.generated.h"

UCLASS()
class MYFIRSTGAME_API AEnemy : public ACharacter, public ICombate_inter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	//int melee_attack();

	int melee_attack_Implementation() override;

	UAnimMontage* get_montage() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* montage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
