// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);

	//不设置这个会自动AddRadialForce,而我们不需要这个效果
	RadialForceComp->SetAutoActivate(false);

	RadialForceComp->Radius = 750.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;

	//如果不设置这个,ImpulseStrength就要非常大才能推动
	RadialForceComp->bImpulseVelChange = true;

	//默认的4种作用类型不包括WorldDynamic
	RadialForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);

}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	RadialForceComp->FireImpulse();

	//第一个参数 -- Log类型
	//第二个参数 -- Log/Warning/Error
	//第三个参数 -- 要打印的信息
	UE_LOG(LogTemp,Log,TEXT("OnActorHit in Explosive Barrel"));

	//注意打印String时加*
	UE_LOG(LogTemp,Warning,TEXT("OtherActor: %s, at game time:%f"),*GetNameSafe(OtherActor),GetWorld()->TimeSeconds);
	FString CombinedString = FString::Printf(TEXT("Hit at location : %s"),*Hit.ImpactPoint.ToString());

	// 第一个和第二个参数已经传递了世界和坐标,第四个参数就不需要指明Actor了,否则只能在Actor上打印字符串
	// 2.0f -- 持续时间 true -- 画出阴影
	DrawDebugString(GetWorld(),Hit.ImpactPoint,CombinedString,nullptr,FColor::Green,2.0f,true);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

