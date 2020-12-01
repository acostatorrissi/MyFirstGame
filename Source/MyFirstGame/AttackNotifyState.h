// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class MYFIRSTGAME_API UAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	void NotifyBegin(USkeletalMeshComponent* const MeshComp, UAnimSequenceBase* const Animation, float const TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* const MeshComp, UAnimSequenceBase* const Animation) override;
	
};
