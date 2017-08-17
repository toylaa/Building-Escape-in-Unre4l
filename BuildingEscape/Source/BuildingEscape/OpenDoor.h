// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Gameframework/Actor.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Called when the game starts
	virtual void BeginPlay() override;

		
	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

		
private:
	

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY(EditAnywhere)
		float TriggerMass = 30.f;
		

		//The Owning Door
		AActor* Owner=nullptr;
		FRotator OwnerStartingRotation;
		FRotator OwnerRotation;
		
		float GetTotalMassOfActorsOnPlate();

	
};
