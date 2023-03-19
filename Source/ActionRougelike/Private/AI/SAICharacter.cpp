// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	// GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents((true));
	
 	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	//默认是PlacedInWorld,那样只有被拖进去的会被AIController控制,换成这个用Spawn生成的也被AIContorller控制
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComp=CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	
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

AActor* ASAICharacter::GetTargetActor()
{
	AAIController * AIC=Cast<AAIController>(GetController());
	if(AIC)
	{
		return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}
	return nullptr;
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	//只在第一次看到时生成感叹号
	if(Pawn!=GetTargetActor() && !GetTargetActor())
	{
		if(ASCharacter* Character=Cast<ASCharacter>(Pawn))
		{
			SetTargetActor(Pawn);

			MulticastPawnSeen();
		}
	}
	
	// DrawDebugString(GetWorld(),GetActorLocation(),"PLAYER SPOTTED",nullptr,FColor::White,4.0f,true);
}

void ASAICharacter::MulticastPawnSeen_Implementation()
{
	USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(),SpottedWidgetClass);
	if(NewWidget)
	{
		NewWidget->AttachedActor = this;

		//调整深度使感叹号显示在其他UI前方
		NewWidget->AddToViewport(10);
	}
}
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float Delta)
{
	if(Delta<0.0f)
	{
		//设置受击效果
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);
		//设置bot受击后锁定玩家
		if(InstigatorActor!=this)
		{
			if(ASCharacter* Character=Cast<ASCharacter>(InstigatorActor))
			{
				SetTargetActor(InstigatorActor);
			}
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

			//设置死亡后胶囊体不产生碰撞,否则会形成一个空气墙
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			//set lifespan
			SetLifeSpan(10.0f);
		}
	}
}


