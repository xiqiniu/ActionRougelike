// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS(Abstract) //abstract -- this class is not complete
class ACTIONROUGELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASProjectileBase();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem * ImpactVFX;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UShapeComponent *SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UProjectileMovementComponent * MoveComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	UParticleSystemComponent * EffectComp;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent * HitComponent,AActor * OtherActor,
		UPrimitiveComponent *OtherComp,FVector NormalImpulse,const FHitResult &Hit);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void Explode();
	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
