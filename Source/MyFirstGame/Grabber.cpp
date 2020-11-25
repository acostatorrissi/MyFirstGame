// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Check for the physics handle component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
		
	}
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	//If we hit something then attach the physics handle
	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation
					(
						ComponentToGrab,
						NAME_None,
						GetPlayersReach(),
						FRotator::ZeroRotator
					);
	}	
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the phisics handle is attach
	if(PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//test
	
	FVector start = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector ForwardVector = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();
	start = start + (ForwardVector * 10.f); //300
	FVector end = start + (ForwardVector * 4000.f); //5000
	
	//Ray-cast
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		start,
		end,
		//GetPlayersWorldPos(),
		//GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	/*
	DrawDebugLine(
		GetWorld(),
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FColor::Green,
		false,
		1,
		0,
		1
	);
	*/
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	//Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayersReach() const
{
	//Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
