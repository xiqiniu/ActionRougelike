// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Action.h"

#include "SActionComponent.h"

void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	//ȷ����instigator��Ҫ�����action
	if(!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent* ActionComp=Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	//ȷ��instigator��ActionComponent
	if(ActionComp)
	{
		//ȷ��Ҫ�����action��instigator��δ���,�������������Ϣ
		if(ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action %s is already known"),*GetNameSafe((ActionToGrant)));
			GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,DebugMsg);
			return;
		}

		//Give new ability
		ActionComp->AddAction(ActionToGrant,InstigatorPawn);
		HideAndCooldownPowerup();
	}	
}
