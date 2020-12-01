// Fill out your copyright notice in the Description page of Project Settings.


#include "Jeep.h"
#include "Components/SphereComponent.h"

// Sets default values
AJeep::AJeep()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereRadius = 300.0f;

	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;

	MyMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("My Mesh"));
	MyMesh->SetupAttachment(RootComponent);

	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AJeep::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AJeep::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJeep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJeep::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( ( OtherActor != nullptr ) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OtherActor->Destroy();
	}
}