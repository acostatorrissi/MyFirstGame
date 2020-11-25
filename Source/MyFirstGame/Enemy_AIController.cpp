// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "MyFirstGameCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"

AEnemy_AIController::AEnemy_AIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/AI/Enemy_BT.Enemy_BT'"));
	if(obj.Succeeded())
	{
		btree = obj.Object;
	}
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	setup_perception_system();	
}


void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
}

void AEnemy_AIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if(blackboard)
	{
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemy_AIController::get_blackboard() const
{
	return blackboard;
}

void AEnemy_AIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AMyFirstGameCharacter>(actor))
	{
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
}


void AEnemy_AIController::setup_perception_system()
{
	//Create and initialize sight config object
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight_config->SightRadius = 500.0f;
	// Enemy won't be able to see player
	sight_config->LoseSightRadius = sight_config->SightRadius + 50.0f;
	// Field of view
	sight_config->PeripheralVisionAngleDegrees = 90.0f;
	//Time after chase stimulus disappear
	sight_config->SetMaxAge(5.0f);
	// Defines how close the source has to stay close to the last location
	sight_config->AutoSuccessRangeFromLastSeenLocation = 900.0f;
	sight_config->DetectionByAffiliation.bDetectEnemies = true;
	sight_config->DetectionByAffiliation.bDetectFriendlies = true;
	sight_config->DetectionByAffiliation.bDetectNeutrals = true;

	// Add sight config comp to perception comp
	GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_AIController::on_target_detected);
	GetPerceptionComponent()->ConfigureSense(*sight_config);
}


