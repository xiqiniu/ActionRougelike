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
	 if(OtherActor && OtherActor!=GetInstigator())
	 {
		//��ȡtTag����һ�ַ�ʽ
	 	// static FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Status.Parrying");
	 	
	 	USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));

			return;
		}
	 	
	 	if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,DamageAmount,SweepResult))
	 	{
	 		Explode();

	 		if(ActionComp)
	 		{
	 			ActionComp->AddAction(BurningActionClass,GetInstigator());
	 		}
	 	}
	 }
}


