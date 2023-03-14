// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API USBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
protected:
	//避免hard code,可以在编辑器中下拉选择对应的key
	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector LowHealthKey;
	UPROPERTY(EditAnywhere,Category="AI")
	float LowHealthFraction;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	USBTService_CheckHealth();
};
