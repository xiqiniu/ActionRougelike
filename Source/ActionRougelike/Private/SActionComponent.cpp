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
	//ֻ��Server��ʼ��Actions
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
	//��Ҫ�ø���ȥɾ����ԭ��: ��ActionEffect���StopAction�����RemoveAction�����Actions���Ƴ�,��ᵼ��forѭ������
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
	
	//��ʾ����ɫ��ǰ��Tag
	// FString DebugMsg=GetNameSafe(GetOwner()) + ":" +ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,DebugMsg);

	//��ʾ���е�Action
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
		//�������������ÿͻ��˺ͷ��������ܻ�ȡ��ActionComp
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
			
			//�˴����õ��߼�,���ͻ��˵���StartActionByName,�����ServerStartAction���Լ�����StartAction,��ʱ�ͻ��˵�bIsRunning�Ѿ�
			//�޸�Ϊtrue,����������ʼ����StartActionByName,�������ǵ��趨,���ᷢһ����֪ͨ���пͻ����޸�bIsRunningΪtrue,����������RPC
			//�Ŀͻ����Ѿ�Ϊtrue,����RepNotify�Ļ���,��ʱ�����ᴥ��OnRep_xxx����,�������ͻ����������Լ���bIsRunning�Ƿ�Ϊfalse����
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
			                
