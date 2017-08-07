// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"



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
	
	//Find the owning actor
	Owner = GetOwner();
	
	//Get Pawn-- save as actor (inherited)
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


void UOpenDoor::OpenDoor()
{	
	//Set Actor rotation +(OpenAngle)
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle , 0.0f));
}

void UOpenDoor::CloseDoor()
{	
		Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Poll the Trigger Volume every frame
	// If the Actor is authorized to Open is in the Volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{//then open door
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	//Check if it is time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

