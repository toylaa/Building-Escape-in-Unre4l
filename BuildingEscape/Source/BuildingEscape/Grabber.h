// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

private:
	float Reach = 150.f;
	bool Grabbed;
	FRotator ActorRotation;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	
	//grab whats in Reach ( Of Ray-cast ) 
	void Grab();
	//Called when grab is released
	void Drop();
	//Rotations
	void RotateRight();
	void RotateLeft();
	//Get START of Reach Line Vector
	FVector GetReachLineStart() const;
	//Get END of Reach Line Vector
	FVector GetReachLineEnd() const;
	//Find PhysicsHandleComponent
	void FindPhysicsHandleComponent();
	//Setup (assumed) attached Input Component
	void SetupInputComponent();
	// Return Hit for first physics body in "Reach"
	FHitResult GetFirstPhysicsBodyInReach() const;		
	
};
