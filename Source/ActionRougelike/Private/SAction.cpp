// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "ActionRougelike/ActionRougelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp,Log,TEXT("Running: %s"),*GetNameSafe(this));
	// LogOnScreen(this,FString::Printf(TEXT("Started: %s"),*ActionName.ToString()),FColor::Green);
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if(GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(),this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp,Log,TEXT("Stopped: %s"),*GetNameSafe(this));
	// LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"),*ActionName.ToString()),FColor::White);
	
	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(),this);
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	//����:�����Ѿ��ڽ�����,��ʱ�������������Ͳ����ε���start;
	if(IsRunning())
	{
		return false;
	}
	
	USActionComponent * Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	return true;
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	AActor* Actor=Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	//���ڷ������Ϳͻ���,�˴�GetOuter()�õ��Ľ����һ��,������Ҫ�޸�
	// return Cast<USActionComponent>(GetOuter());
	
	//Ҳ������ô��
	// AActor* Actor = Cast<AActor>(GetOuter());
	// return Actor->GetComponentByClass(USActionComponent::StaticClass());

	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USAction,RepData);
	DOREPLIFETIME(USAction,ActionComp);
	DOREPLIFETIME(USAction,TimeStarted);
}

           
