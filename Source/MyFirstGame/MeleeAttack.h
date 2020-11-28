// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enemy.h"
#include "MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYFIRSTGAME_API UMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UMeleeAttack(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool montage_has_finished(AEnemy* const enemy);
};
