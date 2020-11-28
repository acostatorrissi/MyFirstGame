// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"
#include "Combate_inter.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine/LatentActionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

UMeleeAttack::UMeleeAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the Enemy
	AAIController* const cont = OwnerComp.GetAIOwner();
	AEnemy* const enemy = Cast<AEnemy>(cont->GetPawn());

	if(ICombate_inter* const icombat = Cast<ICombate_inter>(enemy))
	{
		if(montage_has_finished(enemy))
		{
			icombat->Execute_melee_attack(enemy);
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UMeleeAttack::montage_has_finished(AEnemy* const enemy)
{
	return enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(enemy->get_montage());
}



