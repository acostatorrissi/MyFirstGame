// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Enemy_AIController.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "blackboard_keys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}


EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI controller and its Enemy

	auto const cont = Cast<AEnemy_AIController>(OwnerComp.GetAIOwner());
	auto const enemy = cont->GetPawn();

	// Obtain Enemy location to use as an origin location
	FVector const origin = enemy->GetActorLocation();

	// The output for the random location
	FNavLocation loc;

	// Get the nav system and generate a random location on the nav mesh
	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, loc.Location);
	}

	//Finish with success

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

