// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include <string>

#include "DrawDebugHelpers.h"
#include"EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include"EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),true,TEXT("Enable spawning of bots via timer."),ECVF_Cheat); 

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval=2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 1000;

	//设置GameMode的playerstate,如果不在这里设置也可以在编辑器编辑GameMode相关参数的地方设置
	PlayerStateClass = ASPlayerState::StaticClass();
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
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(),Locations[0],50.0f,20,FColor::Blue,false,60.0f);
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn Powerup EQS Query Failed"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	//Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter=0;
	//Break out if we reached the desired count or if we have no more potential positions remaining;
	while(SpawnCounter<DesiredPowerupCount &&Locations.Num()>0)
	{
		//Pick a random locaiton from remaining points
		int32 RandomLocationIndex = FMath::RandRange(0,Locations.Num()-1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		//Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		//Check minimum disntance requirement
		bool bValidLocation = true;
		for(FVector OtherLocation: UsedLocations)
		{
			float DistanceTo=(PickedLocation-OtherLocation).Size();
			if(DistanceTo<RequiredPowerupDistance)
			{
				//Show skipped locations due to distance
				DrawDebugSphere(GetWorld(), PickedLocation, 50.0f,20,FColor::Red,false,10.0f);

				//too close, skipto next attempt
				bValidLocation=false;
				break;
			}
		}
		//Failed the distance test
		if(!bValidLocation)
		{
			continue;
		}
		//Pick a random PowerupClass
		int32 RandomClassIndex=FMath::RandRange(0,PowerupClasses.Num()-1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass,PickedLocation,FRotator::ZeroRotator);

		//Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
		UE_LOG(LogTemp,Log,TEXT("SpawnCounter=%d"),SpawnCounter);
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

	//一个每隔固定时间就生成bot的定时器
	//实际生成的bot数量受DiffcultyCurve控制
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpwanBotTimerElapsed,SpawnTimerInterval,true);

	//确保至少一种PowerupClass被赋值
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
	//使控制器与当前角色分离,这样再调用RestartPlayer才会创建一个新角色
	Controller->UnPossess();
	
	RestartPlayer(Controller);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp,Log,TEXT("OnActorKilled: Victim: %s, Killer:%s"),*GetNameSafe(VictimActor),*GetNameSafe(Killer));

	//复活玩家
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
		}
	}
}
