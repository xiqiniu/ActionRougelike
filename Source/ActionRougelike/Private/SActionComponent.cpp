// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "ActionRougelike/ActionRougelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

DECLARE_CYCLE_STAT(TEXT("StartActionByName"),STAT_StartActionByName,STATGROUP_STANFORD);

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) const
{
	for(USAction* Action:Actions)
	{
		if(Action && Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	return nullptr;
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();
	//只让Server初始化Actions
	if(GetOwner()->HasAuthority())
	{
		for(TSubclassOf<USAction> ActionClass:DefaultActions)
		{
			AddAction(ActionClass,GetOwner());
		}
	}
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//Stop all
	//需要用副本去删除的原因: 在ActionEffect里的StopAction会调用RemoveAction将其从Actions中移除,这会导致for循环报错
	TArray<USAction*> ActionCopy = Actions;
	for(USAction* Action : ActionCopy)
	{
		if(Action && Action->IsRunning())
		{
			Action->StopAction(GetOwner());
		}
	}
	Super::EndPlay(EndPlayReason);
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//显示各角色当前的Tag
	// FString DebugMsg=GetNameSafe(GetOwner()) + ":" +ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,DebugMsg);

	//显示所有的Action
	for(USAction* Action:Actions)
	{
		FColor TextColor = Action->IsRunning()?FColor::Blue:FColor::White;

		FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"),*GetNameSafe(GetOwner()),*GetNameSafe(Action));
		LogOnScreen(this,ActionMsg,TextColor,0.0f);
	}
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(USAction* Action:Actions)
	{
		if(Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass,AActor* Instigator)
{
	if(!ensure(ActionClass))
	{
		return;
	}
	
	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("Client attempting to AddAction. [Class: %s]"),*GetNameSafe(ActionClass));
		return;
	}
	
	USAction* NewAction = NewObject<USAction>(GetOwner(),ActionClass);
	if(ensure(NewAction))
	{
		//用于网络编程中让客户端和服务器都能获取到ActionComp
		NewAction->Initialize(this);
		Actions.Add(NewAction);
		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if(!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	SCOPE_CYCLE_COUNTER(STAT_StartActionByName);

	for(USAction *Action:Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run :%s"),*ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,FailedMsg);
				continue;
			}
			//Is Client;
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator,ActionName);
			}

			//Bookmark for unreal Insights
			TRACE_BOOKMARK(TEXT("StartAction::%s"),*GetNameSafe(Action));
			
			//此处调用的逻辑,当客户端调用StartActionByName,会调用ServerStartAction并自己调用StartAction,此时客户端的bIsRunning已经
			//修改为true,当服务器开始调用StartActionByName,根据我们的设定,他会发一个包通知所有客户端修改bIsRunning为true,但触发调用RPC
			//的客户端已经为true,根据RepNotify的机制,此时并不会触发OnRep_xxx函数,而其他客户端则会根据自己的bIsRunning是否为false调用
			Action->StartAction(Instigator);
			
			return true;
		}
	}
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(USAction *Action:Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(Action->IsRunning())
			{
				if(!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator,ActionName);
				}
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	return false;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator,ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator,ActionName);
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USActionComponent,Actions);
}
			                
