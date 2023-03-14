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

	//������������Զ�AddRadialForce,�����ǲ���Ҫ���Ч��
	RadialForceComp->SetAutoActivate(false);

	RadialForceComp->Radius = 750.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;

	//������������,ImpulseStrength��Ҫ�ǳ�������ƶ�
	RadialForceComp->bImpulseVelChange = true;

	//Ĭ�ϵ�4���������Ͳ�����WorldDynamic
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

	//��һ������ -- Log����
	//�ڶ������� -- Log/Warning/Error
	//���������� -- Ҫ��ӡ����Ϣ
	UE_LOG(LogTemp,Log,TEXT("OnActorHit in Explosive Barrel"));

	//ע���ӡStringʱ��*
	UE_LOG(LogTemp,Warning,TEXT("OtherActor: %s, at game time:%f"),*GetNameSafe(OtherActor),GetWorld()->TimeSeconds);
	FString CombinedString = FString::Printf(TEXT("Hit at location : %s"),*Hit.ImpactPoint.ToString());

	// ��һ���͵ڶ��������Ѿ����������������,���ĸ������Ͳ���Ҫָ��Actor��,����ֻ����Actor�ϴ�ӡ�ַ���
	// 2.0f -- ����ʱ�� true -- ������Ӱ
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

