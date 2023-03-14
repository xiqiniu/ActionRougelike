// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,AActor*, InstigatorActor,USAttributeComponent*,OwningComp
	,float, NewHealth,float,Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROUGELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="Attributes")
	static USAttributeComponent *GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Attributes",meta = (DisplayName="isAlive"))
	static bool isActorAlive(AActor* Actor);
	USAttributeComponent();

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Health;

	float HealthMax;
public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyHealthChange(AActor *InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable)
	float GetHealthMax();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	
	
};
