// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly,Category="Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly,Category="Teleprot")
	float DetonateDelay;

	//击中物体时清除定时器的句柄
	FTimerHandle TimerHandle_DelayedDetonate;

	//基类中声明为BlueprintNativeEvent,重写时必须加上_Implementation
	virtual  void Explode_Implementation() override;

	void TeleportInstigator();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
