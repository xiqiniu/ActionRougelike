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
	//�����˺�
	DamageAmount=50.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���������Actor�Ҵ��еĶ������ӵ��ķ�����
	 if(OtherActor&&OtherActor!=GetInstigator())
	 {
	 	// //������е�Actor��SAttributeComponent
	 	// USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		 //
	 	// if(AttributeComp)
	 	// {
	 	// 	AttributeComp->ApplyHealthChange(GetInstigator(),-Damage);
		 //
	 	// 	//���к������ӵ�
	 	// 	Destroy();
	 	// }

	 	if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,DamageAmount,SweepResult))
	 	{
	 		Explode();
	 	}
	 }
}


