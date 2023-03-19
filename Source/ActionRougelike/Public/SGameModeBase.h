// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;
class UDataTable;
class USMonsterData;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FMonsterInfoRow: public FTableRowBase
{
	GENERATED_BODY()
public:
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 10.0f;
	}
	// UPROPERTY(EditAnywhere,BlueprintReadOnly)
	// USMonsterData*  MonsterData;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FPrimaryAssetId MonsterId;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float KillReward;
};


UCLASS()
class ACTIONROUGELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	FString SlotName;
	
	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UDataTable* MonsterTable;

	//ʹ��DataTable�����
	// UPROPERTY(EditDefaultsOnly,Category="AI" )
	// TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery *SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat * DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AI")
	float RagePerKill;
	
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

	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);
	
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

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION(BlueprintCallable,Category="SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();
};
