// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	AttackAnimDelay = 0.2f;
	HandSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character=Cast<ACharacter>(Instigator);
	if(Character)
	{
		//播放攻击动画
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect,Character->GetMesh(),HandSocketName,FVector::ZeroVector,FRotator::ZeroRotator,EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"AttackDelay_Elapsed",Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
	}
	
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		//Muzzle_01 -- 手部骨骼的位置,如果没有socket可以打开编辑器addsocket
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//忽略玩家
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams CollisionObjectQueryParams;
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		CollisionObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		//终点
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector()*5000);

		FHitResult Hit;
		//如果击中了blocking的物体
		if(GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,
			CollisionObjectQueryParams,Shape,CollisionQueryParams))
		{
			//击中了物体则修改终点
			TraceEnd=Hit.ImpactPoint;
		}

		FRotator ProjRotation=FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();

		FTransform SpawnTM=FTransform(ProjRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParams);
	}

	StopAction(InstigatorCharacter);
}




