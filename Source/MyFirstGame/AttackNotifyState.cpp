// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterBase.h"
#include "MyFirstGameCharacter.h"

void UAttackNotifyState::NotifyBegin
	(USkeletalMeshComponent* const MeshComp, 
	UAnimSequenceBase* const Animation, 
	float const TotalDuration)
{
	if(MeshComp && MeshComp->GetOwner())
	{
		if(ACharacterBase* const character = Cast<ACharacterBase>(MeshComp->GetOwner()))
		{
			character->attack_start();
		}
		else if(AMyFirstGameCharacter* const player = Cast<AMyFirstGameCharacter>(MeshComp->GetOwner()))
		{
			player->attack_start();
		}
	}
}

void UAttackNotifyState::NotifyEnd(
	USkeletalMeshComponent* const MeshComp, 
	UAnimSequenceBase* const Animation)
{

	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACharacterBase* const character = Cast<ACharacterBase>(MeshComp->GetOwner()))
		{
			character->attack_end();
		}
		else if (AMyFirstGameCharacter* const player = Cast<AMyFirstGameCharacter>(MeshComp->GetOwner()))
		{
			player->attack_end();
		}

	}
}

