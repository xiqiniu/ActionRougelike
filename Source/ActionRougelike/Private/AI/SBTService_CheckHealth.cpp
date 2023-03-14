// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	LowHealthFraction=0.6f;
}
void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//»ñÈ¡BB
	UBlackboardComponent *BlackboardComp=OwnerComp.GetBlackboardComponent();

	if(ensure(BlackboardComp))
	{
		APawn *AIPawn=OwnerComp.GetAIOwner()->GetPawn();
		USAttributeComponent *AttributeComp=USAttributeComponent::GetAttributes(AIPawn);
		bool IsLowHealth = AttributeComp->GetCurrentHealth()/AttributeComp->GetHealthMax()<LowHealthFraction;
		BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName,IsLowHealth);
	}
}


