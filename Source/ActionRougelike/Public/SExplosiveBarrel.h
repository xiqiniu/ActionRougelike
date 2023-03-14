// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
UCLASS()
class ACTIONROUGELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	URadialForceComponent* RadialForceComp;

	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &hit);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
