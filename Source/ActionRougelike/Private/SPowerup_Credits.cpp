// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_Credits.h"

#include "SPlayerState.h"

ASPowerup_Credits::ASPowerup_Credits()
{
	CreditsAmount = 80;
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//�ر���ײ,ʹ�ø����е�SphereComp������ײ 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);
}

void ASPowerup_Credits::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	if(ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditsAmount);
		HideAndCooldownPowerup();
	}
}
