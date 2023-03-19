// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp -> SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this,&ASProjectileBase::OnActorHit);
	RootComponent=SphereComp;

	EffectComp=CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MoveComp=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectiMoveComp");
	MoveComp->bRotationFollowsVelocity=true;
	MoveComp->bInitialVelocityInLocalSpace=true;
	MoveComp->ProjectileGravityScale=0.0f;
	MoveComp->InitialSpeed=2000.0f;

	bReplicates = true;
	// SetReplicates(true);
 
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

//BlueprintNativeEvent定义要在声明的函数名后+_Implementation
void ASProjectileBase::Explode_Implementation()
{
	//调用Destroy()后IsPendingKill会变为true
	//确保不会爆炸两次
	if(ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,ImpactVFX,GetActorLocation(),GetActorRotation());

		Destroy();
	}
}



void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

