// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class ACTIONROUGELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/*
	VisibleAnywhere -- ���Ķ��ܿ���
	VisibleDefaultOnly -- ֻ������ͼ�༭���п���


	EditAnywhere -- �ܿ������ܸ�
	EditDefaultOnly -- ֻ������ͼ�༭���б༭,��ʵ������

	BlueprintReadOnly -- ����ͼ���ܶ�
	BluePrintReadWrite -- ����ͼ���ܶ�д
	*/
	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_Dash;
	FTimerHandle TimerHandle_BlackHoleAttack;

	UPROPERTY(VisibleAnywhere,Category="Effects")
	FName TimeToHitParamName;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;
	
	void MoveForward(float value);
	
	void MoveRight(float value);

	void SprintStart();

	void SprintStop();
	
	void PrimaryAttack();

	void PrimaryInteract();

	void BlackHoleAttack();
	
	void Dash();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor,USAttributeComponent* OwningComp,float NewHealth,float Delta);

	virtual FVector GetPawnViewLocation() const override;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	ASCharacter();

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
};



