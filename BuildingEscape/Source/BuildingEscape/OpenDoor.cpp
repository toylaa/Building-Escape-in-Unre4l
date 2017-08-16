// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Grabber.h"
#define OUT



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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName())
	}
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
	if (GetTotalMassOfActorsOnPlate() > 50.f) //TODO make into a parameter
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

float UOpenDoor::GetTotalMassOfActorsOnPlate() 
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return TotalMass; }
	//find all the overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//Iterate through them adding their masses

	for (const auto* Actor : OverlappingActors) 
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Error, TEXT("%s is on plate"), *Actor->GetName())
	}
	
	return TotalMass;
}

