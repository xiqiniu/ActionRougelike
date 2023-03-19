// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include <string>

#include "DrawDebugHelpers.h"
#include"EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SMonsterData.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "ActionRougelike/ActionRougelike.h"
#include "AI/SAICharacter.h"
#include "Engine/AssetManager.h"
#include"EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer."),ECVF_Cheat); 

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval=2.0f;
	CreditsPerKill = 20;
	RagePerKill = 10.0f;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 1000;

	//����GameMode��playerstate,���������������Ҳ�����ڱ༭���༭GameMode��ز����ĵط�����
	PlayerStateClass = ASPlayerState::StaticClass();

	// SlotName = "SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options,"SaveGame");
	if(SelectedSaveSlot.Len() > 0)
	{
		SlotName = SelectedSaveSlot;
	}
		
	LoadSaveGame();
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	//�Ƽ�������load�Ĺ�������ǰ
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

}

void ASGameModeBase::SpwanBotTimerElapsed()
{
	//����̨������
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	//�������bot����
	int NumOfAliveBots=0;
	for(TActorIterator<ASAICharacter> It(GetWorld());It;++It)
	{
		ASAICharacter* Bot=*It;
		USAttributeComponent * AttributeComp=USAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}
	UE_LOG(LogTemp,Log,TEXT("Found %i alive bots."),NumOfAliveBots);
	float MaxBotCount=10.0f;

	if(DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if(NumOfAliveBots>=MaxBotCount)
	{
		UE_LOG(LogTemp,Log,TEXT("At maximum bot capacity.Skipping bot spawn"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper *QueryInstance =UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if(QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn bot EQS Query Failed!"))
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if(Locations.IsValidIndex(0))
	{
		if(MonsterTable)
		{
				TArray<FMonsterInfoRow*> Rows;
				MonsterTable->GetAllRows("",Rows);

				//Get Random Enemy
				int32 RandomIndex = FMath::RandRange(0,Rows.Num()-1);
				FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

				UAssetManager* Manager = UAssetManager::GetIfValid();
				if(Manager)
				{
						LogOnScreen(this,"Loading monster ...",FColor::Green);
						TArray<FName> Bundles;
						FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this,&ASGameModeBase::OnMonsterLoaded,SelectedRow->MonsterId,Locations[0]);
						Manager->LoadPrimaryAsset(SelectedRow->MonsterId,Bundles,Delegate);
				}
			}
				DrawDebugSphere(GetWorld(),Locations[0],50.0f,20,FColor::Blue,false,60.0f);
		}
	}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this,"Finished Loading",FColor::Green);
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation,
			                                                FRotator::ZeroRotator);
			//����DataAsset���ActionsΪbot���Action
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy : %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				USActionComponent* ActionComp = Cast<USActionComponent>(
					NewBot->GetComponentByClass(USActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<USAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(ActionClass, NewBot);
					}
				}
			}
		}
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	//Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	//Break out if we reached the desired count or if we have no more potential positions remaining;
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		//Pick a random locaiton from remaining points
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		//Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		//Check minimum disntance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPowerupDistance)
			{
				//Show skipped locations due to distance
				DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				//too close, skipto next attempt
				bValidLocation = false;
				break;
			}
		}
		//Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}
		//Pick a random PowerupClass
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		//Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
		UE_LOG(LogTemp, Log, TEXT("SpawnCounter=%d"), SpawnCounter);
	}
}

void ASGameModeBase::KillAllBot()
{
	int NumOfAliveBots=0;
	for(TActorIterator<ASAICharacter> It(GetWorld());It;++It)
	{
		ASAICharacter* Bot=*It;
		USAttributeComponent * AttributeComp=USAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); 
		}
	}
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	//һ��ÿ���̶�ʱ�������bot�Ķ�ʱ��
	//ʵ�����ɵ�bot������DiffcultyCurve����
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpwanBotTimerElapsed,SpawnTimerInterval,true);

	//ȷ������һ��PowerupClass����ֵ
	if(ensure(PowerupClasses.Num()>0))
	{
		//Run EQS to find potential power-up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,PowerupSpawnQuery,this,EEnvQueryRunMode::AllMatching,nullptr);
		if(ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	//ʹ�������뵱ǰ��ɫ����,�����ٵ���RestartPlayer�Żᴴ��һ���½�ɫ
	Controller->UnPossess();
	
	RestartPlayer(Controller);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp,Log,TEXT("OnActorKilled: Victim: %s, Killer:%s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));

	//�������
	ASCharacter * Player= Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());

		float RespawnDelay=2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}

	//Give Credits for kill
	APawn * killerPawn = Cast<APawn>(Killer);
	if(killerPawn)
	{
		if(ASPlayerState *PS=killerPawn->GetPlayerState<ASPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
			USAttributeComponent *AttributeComp=USAttributeComponent::GetAttributes(killerPawn);
			AttributeComp->ApplyRageChange(killerPawn,RagePerKill);
		}
	}
}

void ASGameModeBase::WriteSaveGame()
{
	//Iterate all player states,we don't have proper ID to match yet (requires Steam or EQS)
	for(int32 i = 0;i < GameState->PlayerArray.Num();i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; //single player only at this point
		}
	}
	CurrentSaveGame->SavedActors.Empty();
	
	//Iterate the entire world of actors
	for(FActorIterator It(GetWorld());It;++It)
	{
		AActor *Actor = *It;
		//Only interested in our 'gameplay actors'
		if(!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		//Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		//Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		//Converts Actors SaveGame UPROPERTIES into binary array;
		Actor->Serialize(Ar);
		CurrentSaveGame->SavedActors.Add(ActorData);;
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void ASGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if(CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Failed to load SaveGame Data."));
			return;
		}
		UE_LOG(LogTemp,Log,TEXT("Loaded SaveGame Data."));

		//Iterate the entire world of actors
		for(FActorIterator It(GetWorld());It;++It)
		{
			AActor *Actor = *It;
			//Only interested in our 'gameplay actors'
			if(!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}
			for(FActorSaveData ActorData:CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					//Use the binary array to fill the array
					FMemoryReader MemReader(ActorData.ByteData);
		
					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);
					//Find only variables with UPROPERTY(SaveGame)
					Ar.ArIsSaveGame = true;
					//Converts binary array into Actor's SaveGame PROPERTIES
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp,Log,TEXT("Created New SaveGame Data."));
	}
}
