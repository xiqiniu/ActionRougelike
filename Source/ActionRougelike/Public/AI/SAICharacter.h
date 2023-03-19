// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

class USWorldUserWidget;
UCLASS()
class ACTIONROUGELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	UFUNCTION(NetMulticast,Unreliable)
	void MulticastPawnSeen();
	
	USWorldUserWidget* ActiveHealthBar;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditAnywhere,Category="UI")
	TSubclassOf<USWorldUserWidget> SpottedWidgetClass;
	
	void SetTargetActor(AActor *NewTarget);
	UPROPERTY(VisibleAnywhere,Category="AI")
	UPawnSensingComponent * PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent *AttributeComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent *ActionComp;
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,USAttributeComponent* OwningComp,float NewHealth,float Delta);
	
	UFUNCTION()
	void OnSeePawn(APawn * Pawn);

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor();
};
