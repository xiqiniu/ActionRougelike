// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROUGELIKE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:	
	ASMagicProjectile();

protected:

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	// USphereComponent* SphereComp;
	//
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	// UProjectileMovementComponent* MoveComp;
	//
	// UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	// UParticleSystemComponent* EffectComp;

public:	

	UPROPERTY(EditAnywhere,Category="Damage")
	float DamageAmount;

};
