// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	UPROPERTY(EditAnywhere,Category="Powerup")
	float ReSpawnTime;

	FTimerHandle TimerHandle_ReSpawnTimer;

	UFUNCTION()
	void ShowPowerUp();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool BNewIsActive);

	UPROPERTY(VisibleAnywhere,Category="Components")
	USphereComponent *SphereComp;
public:	
	void Interact_Implementation(APawn* InstigatorPawn) override;
};
