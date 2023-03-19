// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	CreditCost=30;
} 

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent *AttributeComp=Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass((USAttributeComponent::StaticClass())));

	if(ensure(AttributeComp)&&!AttributeComp->IsFullHealth())
	{
		// DrawDebugString(GetWorld(),GetActorLocation(),"POTION",nullptr,FColor::White,4.0f,true);
		if(ASPlayerState *PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if(PS->RemoveCredits(CreditCost)&&AttributeComp->ApplyHealthChange(this,AttributeComp->GetHealthMax()))
			{ 
				HideAndCooldownPowerup();
			}
		}
	}
}

FText ASPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent *AttributeComp=Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass((USAttributeComponent::StaticClass())));
	if(AttributeComp && AttributeComp -> IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning","Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage","Cost {0} Credits. Restore to maximum"),CreditCost);

}

#undef LOCTEXT_NAMESPACE 