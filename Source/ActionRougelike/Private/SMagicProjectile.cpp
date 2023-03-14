// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SCharacter.h"
#include "Components\SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnActorOverlap);
	//设置伤害
	DamageAmount=50.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//如果打中了Actor且打中的对象不是子弹的发射者
	 if(OtherActor&&OtherActor!=GetInstigator())
	 {
	 	// //如果打中的Actor有SAttributeComponent
	 	// USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		 //
	 	// if(AttributeComp)
	 	// {
	 	// 	AttributeComp->ApplyHealthChange(GetInstigator(),-Damage);
		 //
	 	// 	//打中后销毁子弹
	 	// 	Destroy();
	 	// }

	 	if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,DamageAmount,SweepResult))
	 	{
	 		Explode();
	 	}
	 }
}


