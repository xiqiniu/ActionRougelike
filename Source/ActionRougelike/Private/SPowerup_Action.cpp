// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Action.h"

#include "SActionComponent.h"

void ASPowerup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	//确保有instigator和要给予的action
	if(!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	USActionComponent* ActionComp=Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(USActionComponent::StaticClass()));
	//确保instigator有ActionComponent
	if(ActionComp)
	{
		//确保要给予的action该instigator还未获得,否则输出错误信息
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
