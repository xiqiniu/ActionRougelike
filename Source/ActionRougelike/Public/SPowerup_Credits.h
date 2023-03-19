// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_Credits.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPowerup_Credits : public ASPowerupActor
{
	GENERATED_BODY()
public:
	ASPowerup_Credits();
protected:
	UPROPERTY(EditDefaultsOnly,Category="Credits")
	int32 CreditsAmount;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
