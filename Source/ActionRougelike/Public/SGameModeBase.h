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

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	UEnvQuery* PowerupSpawnQuery;

	//All power up classes used to spawn with EQS at match start
	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	//Distance required between power-up spawn locations
	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	float RequiredPowerupDistance;

	//Amount of powerups to spawn during match start
	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	int32 DesiredPowerupCount;

	UFUNCTION()
	void SpwanBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
public:
	virtual void OnActorKilled(AActor *VictimActor,AActor *Killer);
	
	ASGameModeBase();
	virtual  void StartPlay() override;

	UFUNCTION(Exec)
	void KillAllBot();
	
	
};
