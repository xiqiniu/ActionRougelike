// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTargetDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USAttributeComponent * AttributeCommp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,USAttributeComponent* OwningComp,float NewHealth,float Delta);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
