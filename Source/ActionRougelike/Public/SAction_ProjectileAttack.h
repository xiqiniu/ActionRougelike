// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category="Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAnim;
	
	UPROPERTY(EditAnywhere,Category="Attack")
	UParticleSystem *CastingEffect;

	UPROPERTY(EditAnywhere,Category="Attack")
	float AttackAnimDelay;

	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName HandSocketName;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	USAction_ProjectileAttack();
};
