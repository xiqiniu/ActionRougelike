// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	SphereComp=CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent=SphereComp;

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	//关闭碰撞,使用父类中的SphereComp处理碰撞 
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	bIsActive = true;
	ReSpawnTime = 10.0f;

	bReplicates = true;
	// SetReplicates(true);
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
	bIsActive = BNewIsActive;
	OnRep_IsActive();
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	ISGameplayInterface::Interact_Implementation(InstigatorPawn);

}

FText ASPowerupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	//注意第二个是设置其可见性是否影响其子项的可见性
	RootComponent->SetVisibility(bIsActive,true);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor,bIsActive);
}
                
