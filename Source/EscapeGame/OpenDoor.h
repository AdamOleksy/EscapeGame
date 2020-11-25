// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UENUM()
enum class DoorSide : int32
{
	Left = 90,
	Right = -90
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float TargetDoorAngle;
	float CurrentDoorAngle;
	float StatrPosition;
	float DoorLastOpened{0};

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay{1.f};

	UPROPERTY(EditAnywhere)
	DoorSide OpeningSide{DoorSide::Left};

	UPROPERTY(EditAnywhere)
	float OpeningSpeed;

	UPROPERTY(EditAnywhere)
	float ClosingSpeed{2.f};

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpens;
};
