// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	SphereComp=CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent=SphereComp;
	ReSpawnTime=10.0f;
}


void ASPowerupActor::ShowPowerUp()
{
	SetPowerupState((true));
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_ReSpawnTimer,this,&ASPowerupActor::ShowPowerUp,ReSpawnTime);
}

void ASPowerupActor::SetPowerupState(bool BNewIsActive)
{
	SetActorEnableCollision(BNewIsActive);
	//注意第二个是设置其可见性是否影响其子项的可见性
	RootComponent->SetVisibility(BNewIsActive,true);
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);
}

