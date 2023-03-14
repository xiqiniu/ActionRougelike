// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASDashProjectile::ASDashProjectile()
{
	TeleportDelay =0.2f;
	DetonateDelay=0.2f;
	MoveComp->InitialSpeed=4000.0f;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASDashProjectile::Explode_Implementation()
{
	//不调用基类的Explode(会调用Destroy())
	//Super::Explode_Implementation()
	//如果爆炸已经调用过(如通过OnActorHit),清除定时器
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());

	EffectComp->DeactivateSystem();
	MoveComp->StopMovementImmediately();

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport,this,&ASDashProjectile::TeleportInstigator,TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor * ActorToTeleport = GetInstigator();
	if(ensure(ActorToTeleport))
	{
		ActorToTeleport->TeleportTo(GetActorLocation(),ActorToTeleport->GetActorRotation(),false,false);
	}
}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate,this,&ASDashProjectile::Explode,DetonateDelay);
	
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

