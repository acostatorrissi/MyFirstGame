// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "blackboard_keys.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get TargetLocation from blackboard via AI Controller
	AEnemy_AIController* const cont = Cast<AEnemy_AIController>(OwnerComp.GetAIOwner());
	FVector const player_location = cont->get_blackboard()->GetValueAsVector(bb_keys::target_location);

	//Move to the player's location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, player_location);

	//Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}



