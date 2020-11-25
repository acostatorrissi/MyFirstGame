// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Enemy_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "blackboard_keys.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPlayerLocation");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get player character and the Enemy's controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AEnemy_AIController>(OwnerComp.GetAIOwner());

	// Get player location to use as an origin
	FVector const player_location = player->GetActorLocation();

	if (search_random)
	{
		FNavLocation loc;

		// Get the nav system and generate a random location near the player
		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if(nav_sys->GetRandomPointInNavigableRadius(player_location, search_radius, loc, nullptr))
		{
			cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, loc.Location);
		}
	}
	else
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, player_location);
	}

	// Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
	return EBTNodeResult::Succeeded;
}