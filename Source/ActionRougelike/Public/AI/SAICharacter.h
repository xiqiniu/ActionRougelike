// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

UCLASS()
class ACTIONROUGELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	USWorldUserWidget* ActiveHealthBar;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParamName;
	
	void SetTargetActor(AActor *NewTarget);
	UPROPERTY(VisibleAnywhere,Category="AI")
	UPawnSensingComponent * PawnSensingComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent *AttributeComp;
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,USAttributeComponent* OwningComp,float NewHealth,float Delta);
	
	UFUNCTION()
	void OnSeePawn(APawn * Pawn);


public:	



};
