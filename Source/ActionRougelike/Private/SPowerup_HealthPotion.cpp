// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"

#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//�ر���ײ,ʹ�ø����е�SphereComp������ײ 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
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
		DrawDebugString(GetWorld(),GetActorLocation(),"POTION",nullptr,FColor::White,4.0f,true);
		if(AttributeComp->ApplyHealthChange(this,AttributeComp->GetHealthMax()))
		{ 
			HideAndCooldownPowerup();
		}
	}
}
