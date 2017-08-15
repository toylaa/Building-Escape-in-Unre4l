// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h" 
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


void UGrabber::FindPhysicsHandleComponent()
{
	///Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle==nullptr) {UE_LOG(LogTemp, Error, TEXT("Physics Handle NOT Attached")) }
}


void UGrabber::SetupInputComponent()
{
	///Look for attached InputComponent (Only appears at Run Time) 
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{	
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
		InputComponent->BindAction("Spin UP", IE_Pressed, this, &UGrabber::RotateRight);
		InputComponent->BindAction("Spin DWN", IE_Pressed, this, &UGrabber::RotateLeft);
	}
	else{UE_LOG(LogTemp, Error, TEXT("%s missing Input Component"), *GetOwner()->GetName())}
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	
	///Setup Query Parameters 
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line Trace (Ray-Cast) out to reach distance
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}

void UGrabber::Grab()
{
	///LINE-TRACE and and "reach" any actors with physics body collision channel set 
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();	
	auto ActorHit = HitResult.GetActor();			
		
	/// If no actor is hit - Do nothing	
	if (!ActorHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Whiff.."))
	}			
	/// If Actor is HIT - GRAB Component	
	else if(ActorHit)
		{
			
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				ComponentToGrab,	//
				NAME_None,			//No bones needed
				ComponentToGrab->GetOwner()->GetActorLocation(),
				FRotator(0)
			);
		//Rotation Control variable set. LOGIC = Only rotate if object is successfully grabbed
		Grabbed = true;
		//Reference current Actor Rotation (FRotator)
		ActorRotation = ActorHit->GetActorRotation();
		}
}

void UGrabber::Drop()
{
	//Release Physics handle
	PhysicsHandle->ReleaseComponent();
	//Grabbed ~ Can Rotate = FALSE once dropped
	Grabbed = false;
}

void UGrabber::RotateRight()
{
	if (Grabbed)
	{				
		//FRotator Set Yaw + (20) rotation interval 
		ActorRotation.Yaw = ActorRotation.Yaw + 20.f;		
		//Rotate Right
		PhysicsHandle->SetTargetRotation(ActorRotation);		
	}
}

void UGrabber::RotateLeft()
{
	if (Grabbed)
	{	
		//FRotator Set Yaw - (20) rotation interval 
		ActorRotation.Yaw = ActorRotation.Yaw - 20.f;
		// Rotate Left
		PhysicsHandle->SetTargetRotation(ActorRotation);		
	}
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}


FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//LINE TRACE END POINT at which Grabbed objects are "held"
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = GetReachLineEnd();
	///if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		///Move the object that were holding 
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

