// Fill out your copyright notice in the Description page of Project Settings.


#include "IsPlayerInMeleeRange.h"
#include "Enemy_AIController.h"
#include "Enemy.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "blackboard_keys.h"

UIsPlayerInMeleeRange::UIsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UIsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// Get the enemy
	AEnemy_AIController* const cont = Cast<AEnemy_AIController>(OwnerComp.GetAIOwner());
	AEnemy* const enemy = Cast<AEnemy>(cont->GetPawn());

	// Get player character
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Write true or false depending on whether the player is within melee_range
	cont->get_blackboard()->SetValueAsBool(bb_keys::player_is_in_melee_range, enemy->GetDistanceTo(player) <= melee_range);
}


