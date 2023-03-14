// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//获取对应的BB
	UBlackboardComponent * BlackboardComp=OwnerComp.GetBlackboardComponent();

	if(ensure(BlackboardComp))
	{
		//获取玩家
		AActor *TargetActor=Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			//获取AI控制器
			AAIController * MyController=OwnerComp.GetAIOwner();
			if(ensure(MyController))
			{
				APawn *AIPawn=MyController->GetPawn();
				if(ensure(AIPawn))
				{
					//获取AI角色与玩家角色的距离
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());

					//判断距离
					bool bWithinRange = DistanceTo<2000.f;

					//判断有无阻挡
					bool bHasLOS=MyController->LineOfSightTo(TargetActor);
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,bWithinRange&&bHasLOS);
				}
			}
		}
		
	}
}
