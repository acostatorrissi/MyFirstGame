// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFirstGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "HealthBar.h"
//#include "Runtime/Engine/Classes/Engine/World.h"
//#include "Components/SkeletalMeshComponent.h" 
//#include "Materials/MaterialInstanceDynamic.h"
//#include "GameFramework/PlayerController.h"



//////////////////////////////////////////////////////////////////////////
// AMyFirstGameCharacter

AMyFirstGameCharacter::AMyFirstGameCharacter() :
	health(max_health),
	widget_component(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	bFirstPersonCamera = false;
	SwitchCamera();

	setup_stimulus();

	if(widget_component)
	{
		widget_component->SetupAttachment(RootComponent);
		widget_component->SetWidgetSpace(EWidgetSpace::Screen);
		widget_component->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));
		static ConstructorHelpers::FClassFinder<UUserWidget> widget_class(TEXT("/Game/UI/HealthBar_BP"));
		if(widget_class.Succeeded())
		{
			widget_component->SetWidgetClass(widget_class.Class);
		}
	}
}

void AMyFirstGameCharacter::Switch()
{
	if(bFirstPersonCamera)
	{
		bFirstPersonCamera = false;
	}
	else
	{
		bFirstPersonCamera = true;
	}
	SwitchCamera();
}

void AMyFirstGameCharacter::SwitchCamera()
{
	if(bFirstPersonCamera)
	{
		CameraBoom->SetRelativeLocation(FVector(23.0f, 0, 67.0f));
		CameraBoom->TargetArmLength = 0;
		bUseControllerRotationYaw = true;
		GetMesh()->bOwnerNoSee = true;
		GetMesh()->MarkRenderStateDirty();
	}
	else
	{
		CameraBoom->SetRelativeLocation(FVector(0, 0, 0));
		CameraBoom->TargetArmLength = 300.0f;
		bUseControllerRotationYaw = false;
		GetMesh()->bOwnerNoSee = false;
		GetMesh()->MarkRenderStateDirty();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyFirstGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("SwitchView", IE_Pressed, this, &AMyFirstGameCharacter::Switch);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyFirstGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyFirstGameCharacter::MoveRight);

	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyFirstGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyFirstGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyFirstGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyFirstGameCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyFirstGameCharacter::OnResetVR);
}


void AMyFirstGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyFirstGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyFirstGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyFirstGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyFirstGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyFirstGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyFirstGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyFirstGameCharacter::setup_stimulus()
{
	// Create stimulus
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

float AMyFirstGameCharacter::get_health() const
{
	return health;
}

void AMyFirstGameCharacter::set_health(float const new_health)
{
	health = new_health;
}

void AMyFirstGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto const uw = Cast<UHealthBar>(widget_component->GetUserWidgetObject());
	if(uw)
	{
		uw->set_bar_value_percent(health / max_health);
	}
}

float AMyFirstGameCharacter::get_max_health() const
{
	return max_health;
}




