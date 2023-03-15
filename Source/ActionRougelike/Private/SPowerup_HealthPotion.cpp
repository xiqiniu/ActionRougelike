// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//关闭碰撞,使用父类中的SphereComp处理碰撞 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

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
