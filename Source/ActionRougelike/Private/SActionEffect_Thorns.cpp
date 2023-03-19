// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f;
	Duration = 0.0f;
	Period=0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	//Start listening
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if(AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this,&USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	//Stop listening,����������뿪��Ϸʱ
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if(AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this,&USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* Instigator, USAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	//���Լ�����˺�������
	if(Delta<0.0f && OwningActor != Instigator)
	{
		int32 ReflectedAmount = FMath::RoundToInt(Delta*ReflectFraction);
		if(ReflectedAmount == 0)
		{
			return;
		}
		//תΪ����
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		USGameplayFunctionLibrary::ApplyDamage(OwningActor,Instigator,ReflectedAmount);
	}
}