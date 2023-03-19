// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVARDamageMultiplier(TEXT("su.DamageMultiplier"),1.0f,TEXT("Global Damage Modifier for Attribute Component."),ECVF_Cheat);
// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax=100;
	Health=HealthMax;
	RageMax=100;
	Rage=0;
	SetIsReplicatedByDefault(true);
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
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health+Delta,0.0f,HealthMax);
	float ActualDelta=NewHealth-OldHealth;

	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		//ֻ���иı��ʱ�����,��Լ��Դ
		//�鲥��صĶ�Ӧ�÷���Server
		if(ActualDelta!=0.0f)
		{
			MulticastHealthChanged(InstigatorActor,Health,ActualDelta);
		}

		//Died,GameMode��صĶ�Ӧ�÷���Server
		if(ActualDelta<0.0f&&Health == 0.0f)
		{
			ASGameModeBase *GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if(GM)
			{
				GM->OnActorKilled(GetOwner(),InstigatorActor);
			}
		}
	}
	return ActualDelta!=0;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	//��¼�ı�ǰ������ֵ,���ڼ�����ʵ�ĸı�ֵ
	float OldRage=Rage;
	
	Rage=FMath::Clamp(Rage+Delta,0.0f,RageMax);

	float ActualDelta=Rage-OldRage;

	OnRageChanged.Broadcast(InstigatorActor,this,Rage,ActualDelta);
	
	return ActualDelta!=0;
}

float USAttributeComponent::GetRageMax()
{
	return RageMax;
}

float USAttributeComponent::GetCurrentRage()
{
	return Rage;
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

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent,Health);
	DOREPLIFETIME(USAttributeComponent,HealthMax);
	
	//�����Ż�,����ʲôʱ�����Ҫͬ��
	// DOREPLIFETIME_CONDITION(USAttributeComponent,HealthMax,COND_OwnerOnly);
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this,NewHealth,Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor,this,NewRage,Delta);
}


