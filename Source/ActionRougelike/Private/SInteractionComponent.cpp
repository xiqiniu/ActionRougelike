// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"),false,TEXT("Enable Debug Lines for Interact Component."),ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	bool bDebugDraw=CVarDebugDrawInteraction.GetValueOnGameThread();
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor * MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);

	FVector End = EyeLocation+(EyeRotation.Vector()*1000);
	
	//第一种实现--LineTrace 线检测
	// FHitResult Hit;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit,EyeLocation,End,ObjectQueryParams);
	//
	// AActor* HitActor= Hit.GetActor();
	// if(HitActor)
	// {
	// 	if (HitActor->Implements<USGameplayInterface>())
	// 	{
	// 		//这个转型是安全的
	// 		APawn* MyPawn=Cast<APawn>(MyOwner);
	//
	// 		//第一个参数是调用接口的对象,因为在接口中定义的参数是Pawn,所以第二个参数需要转型
	// 		ISGameplayInterface::Execute_Interact(HitActor,MyPawn);
	// 	}
	// }
	// FColor LineColor = bBlockingHit?FColor::Green : FColor::Red;
	// DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,2.0f,0,2.0f);

	//第二种实现 -- 球体检测,从起点发出一个球移动到终点,返回中途碰到的物体
	//FQquat::Identity -- 不增加任何旋转
	TArray<FHitResult> Hits;
	float Radius = 50.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits,EyeLocation,End,FQuat::Identity,ObjectQueryParams,Shape);
	FColor LineColor = bBlockingHit?FColor::Green : FColor::Red;
	for(FHitResult Hit:Hits)
	{
		AActor* HitActor= Hit.GetActor();
		if(bDebugDraw)
		{
			DrawDebugSphere(GetWorld(),Hit.ImpactPoint,Radius,32,LineColor,false,2.0f);
		}
		if(HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				//这个转型是安全的
				APawn* MyPawn=Cast<APawn>(MyOwner);
		
				//第一个参数是调用接口的对象,因为在接口中定义的参数是Pawn,所以第二个参数需要转型
				ISGameplayInterface::Execute_Interact(HitActor,MyPawn);
				
				//我们希望一次操作只能与一个物体互动
				break;
			}
		}
	}
	if(bDebugDraw)
	{
		DrawDebugLine(GetWorld(),EyeLocation,End,LineColor,false,2.0f,0,2.0f);
	}
}