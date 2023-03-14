// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include"EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include"EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer."),ECVF_Cheat); 

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval=2.0f;
}

void ASGameModeBase::SpwanBotTimerElapsed()
{
	//控制台作弊码
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	//设置最大bot数量
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
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
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(),Locations[0],50.0f,20,FColor::Blue,false,60.0f);
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

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpwanBotTimerElapsed,SpawnTimerInterval,true);
	
}
void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	//是控制器与当前角色分离,这样再调用RestartPlayer才会创建一个新角色
	Controller->UnPossess();
	
	RestartPlayer(Controller);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter * Player= Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());

		float RespawnDelay=2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}

	UE_LOG(LogTemp,Log,TEXT("OnActorKilled: Victim: %s, Killer:%s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));
}
