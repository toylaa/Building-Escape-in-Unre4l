// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h" 
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
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
	if (PhysicsHandle) {/*Do nothing*/ }
	else { UE_LOG(LogTemp, Error, TEXT("%s Missing Physics Handle Component"), *GetOwner()->GetName()); }
}


void UGrabber::SetupInputComponent()
{
	///Look for attached InputComponent (Only appears at Run Time) 
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"));
		///Bind the Input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Drop);
	}
	else{UE_LOG(LogTemp, Error, TEXT("%s missing Input Component"), *GetOwner()->GetName());}
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	
	
	// Get the Players view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	///Setup Query Parameters 
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	///Line Trace (Ray-Cast) out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	///See what we hit 
	AActor * ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line-Trace HIT--> %s :)"), *(ActorHit->GetName()))
	}
	///Get on with it...		
return Hit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("** Grab **"));

	///LINE-TRACE and and "reach" any actors with physics body collision channel set 
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();	
	auto ActorHit = HitResult.GetActor();
	

		UE_LOG(LogTemp, Warning, TEXT("** Actor hit : %s**"), *ActorHit->GetName());
		// GrabComponent Docs ----- // 
		//	void GrabComponentAtLocation(class UPrimitiveComponent* Component, FName InBoneName, FVector GrabLocation);
		//	void GrabComponentAtLocationWithRotation(class UPrimitiveComponent* Component, FName InBoneName, FVector Location, FRotator Rotation);
	
	
		if (PhysicsHandle == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("NULL PTR! YOU MESSED UP THE POINTERS !! --> %s"), *(ActorHit->GetName()))
		}
		
		else
		{
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				ComponentToGrab,
				NAME_None,
				ComponentToGrab->GetOwner()->GetActorLocation(),
				FRotator(0)
			);
		}

	


}

void UGrabber::Drop()
{
	UE_LOG(LogTemp, Warning, TEXT("** Drop **"));

	//TODO Release Physics handle

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	///if the physics handle is attached
		//Move the object that were holding 

	

}

