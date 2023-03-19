// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()
public:
	USActionEffect_Thorns();
protected:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	UPROPERTY(EditDefaultsOnly,Category="Effect")
	float ReflectFraction;

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(AActor* Instigator, USAttributeComponent* OwningComp,float NewHealth,float Delta);
};


