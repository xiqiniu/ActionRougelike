// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree,TEXT("Behavior Tree is a nullptr!Please assign it in AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// APawn * MyPawn=UGameplayStatics::GetPlayerPawn(this,0);
	// if(MyPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsVector("MovetoLocation",MyPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	// }
}
