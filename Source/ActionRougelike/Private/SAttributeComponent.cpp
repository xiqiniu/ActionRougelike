// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVARDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f,TEXT("Global Damage Modifier for Attribute Component."),ECVF_Cheat);
// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax=100;
	Health=HealthMax;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-GetHealthMax());
}  

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::isActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp=GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::IsAlive() const
{
	return Health>0.0f;
}

bool USAttributeComponent::ApplyHealthChange(AActor * InstigatorActor,float Delta)
{
	//��������ģʽ &&Delta<0.0f��ֹ����godģʽ���ܻ�Ѫ
	if(!GetOwner()->CanBeDamaged()&&Delta<0.0f)
	{
		return false;
	}
	if(Delta<0.0f)
	{
		float DamageMultiplier = CVARDamageMultiplier.GetValueOnGameThread();
		
		Delta *=DamageMultiplier;
	}
	//��¼�ı�ǰ������ֵ,���ڼ�����ʵ�ĸı�ֵ
	float OldHealth=Health;
	
	Health=FMath::Clamp(Health+Delta,0.0f,HealthMax);

	float ActualDelta=Health-OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);

	//Died
	if(ActualDelta<0.0f&&Health == 0.0f)
	{
		ASGameModeBase *GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(),InstigatorActor);
		}
	}
	
	return ActualDelta!=0;
}

bool USAttributeComponent::IsFullHealth()
{
	return Health==HealthMax;
}

float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

float USAttributeComponent::GetCurrentHealth()
{
	return Health;
}







