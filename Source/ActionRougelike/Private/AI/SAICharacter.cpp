// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents((true));
	
 	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	//Ĭ����PlacedInWorld,����ֻ�б��Ͻ�ȥ�ĻᱻAIController����,���������Spawn���ɵ�Ҳ��AIContorller����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComp=CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	TimeToHitParamName="TimeToHit";
	
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController * AIC=Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
	}
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this,&ASAICharacter::OnSeePawn);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	// DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER SPOTTED",nullptr,FColor::White,4.0f,true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
								  float Delta)
{
	if(Delta<0.0f)
	{
		//�����ܻ�Ч��
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);
		//����bot�ܻ����������
		if(InstigatorActor!=this)
		{
			SetTargetActor(InstigatorActor);
		}

		if(ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		if(NewHealth<=0.0f)
		{
			//stop BT
			AAIController *AIC=Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic(("killed"));
			}

			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			//�������������岻������ײ,������γ�һ������ǽ
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}


