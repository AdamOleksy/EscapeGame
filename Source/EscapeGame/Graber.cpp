// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Graber.h"

#define OUT

// Sets default values for this component's properties
UGraber::UGraber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGraber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupImputComponent();
}


// Called every frame
void UGraber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult = GetFirstPhysicsBodyInRach();

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation);
	
	FVector TraceLineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(TraceLineEnd);	
	}
}


void UGraber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grab"));

	FHitResult HitResult = GetFirstPhysicsBodyInRach();

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation);
	
	FVector TraceLineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if(HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
			(
				ComponentToGrab,
				NAME_None,
				TraceLineEnd
			);
	}
}


void UGraber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Release"));
	PhysicsHandle->ReleaseComponent();
}

void UGraber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error in %s, there is no UPhysicsHandleComponent attach"), *GetOwner()->GetName() );
		UE_LOG(LogTemp, Error, TEXT("Open blueprint and attach Physics Handle component"));
	}
}

void UGraber::SetupImputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGraber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGraber::Release);
	}

}

FHitResult UGraber::GetFirstPhysicsBodyInRach() const
{
	// Get player wieport
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation);

	FVector TraceLineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	FHitResult Hit;
	FCollisionQueryParams TraceParams
		(
			FName(TEXT("")),
			false,
			GetOwner()
		);

	GetWorld()->LineTraceSingleByObjectType
		(
			OUT Hit,
			PlayerViewPointLocation,
			TraceLineEnd,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			TraceParams
		);

	AActor* HitActor = Hit.GetActor();
	if(HitActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Name %s"), *HitActor->GetName() );
	}

	return Hit;
}