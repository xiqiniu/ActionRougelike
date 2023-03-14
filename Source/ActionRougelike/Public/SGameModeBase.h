// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category="AI" )
	TSubclassOf<AActor> MinionClass;
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery *SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat * DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpwanBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
public:
	virtual void OnActorKilled(AActor *VictimActor,AActor *Killer);
	
	ASGameModeBase();
	virtual  void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllBot();
	
	
};
