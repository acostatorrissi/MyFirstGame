// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "MyFirstGameCharacter.h"

// Sets default values
AEnemy::AEnemy() :
	health(max_health),
	widget_component(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	if(widget_component)
	{
		widget_component->SetupAttachment(RootComponent);
		widget_component->SetWidgetSpace(EWidgetSpace::Screen);
		widget_component->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));
		static ConstructorHelpers::FClassFinder<UUserWidget>widget_class(TEXT("/Game/UI/HealthBar_BP"));
		if(widget_class.Succeeded())
		{
			widget_component->SetWidgetClass(widget_class.Class);
		}
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (right_fist_collision_box)
	{
		FAttachmentTransformRules const rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);

		right_fist_collision_box->AttachToComponent(GetMesh(), rules, "hand_r_socket");
		right_fist_collision_box->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
	}

	// Attach delegates
	if(right_fist_collision_box)
	{
		right_fist_collision_box->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::on_attack_overlap_begin);
		right_fist_collision_box->OnComponentEndOverlap.AddDynamic(this, &AEnemy::on_attack_overlap_end);
	}	
}

void AEnemy::on_attack_overlap_begin(UPrimitiveComponent* const overlapped_component, AActor* const other_actor, UPrimitiveComponent* other_component, int const other_body_index, bool const from_sweep, FHitResult const& sweep_result)
{
	if(AMyFirstGameCharacter* const player = Cast<AMyFirstGameCharacter>(other_actor))
	{
		float const new_health = player->get_health() - player->get_max_health() * 0.07f;
		player->set_health(new_health);
	}
}

void AEnemy::on_attack_overlap_end(UPrimitiveComponent* const overlapped_component, AActor* const other_actor, UPrimitiveComponent* other_component, int const other_body_index)
{
	
}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto const uw = Cast<UHealthBar>(widget_component->GetUserWidgetObject());
	if(uw)
	{
		uw->set_bar_value_percent(health / max_health);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::get_health() const
{
	return health;
}

float AEnemy::get_max_health() const
{
	return max_health;
}

void AEnemy::set_health(float const new_health)
{
	health = new_health;
	if(health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("You win!!"));
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}

UAnimMontage* AEnemy::get_montage() const
{
	return montage;
}


void AEnemy::melee_attack()
{
	if(montage)
	{
		PlayAnimMontage(montage);
	}
}



