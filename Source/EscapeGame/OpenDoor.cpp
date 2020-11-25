// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	TargetDoorAngle = static_cast<float>(OpeningSide);
	CurrentDoorAngle = 0.f;
	StatrPosition = GetOwner()->GetActorRotation().Yaw;

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("No presure plate have been assigned to %s"), *GetOwner()->GetName()) ;
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if(GetWorld()->GetTimeSeconds()-DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}


//Function called to open door
void UOpenDoor::OpenDoor(float DeltaTime)
{	
	CurrentDoorAngle = FMath::FInterpTo(CurrentDoorAngle, TargetDoorAngle, DeltaTime, OpeningSpeed);
	GetOwner()->SetActorRotation({0.f, StatrPosition+CurrentDoorAngle, 0.f});
}


//Function called to close door
void UOpenDoor::CloseDoor(float DeltaTime)
{	
	CurrentDoorAngle = FMath::FInterpTo(CurrentDoorAngle, 0.f, DeltaTime, ClosingSpeed);
	GetOwner()->SetActorRotation({0.f, StatrPosition+CurrentDoorAngle, 0.f});
}