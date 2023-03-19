// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"
class USAttributeComponent;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,AActor*, InstigatorActor,USAttributeComponent*,OwningComp
// 	,float, NewHealth,float,Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged,AActor*, InstigatorActor,USAttributeComponent*,OwningComp
	,float, NewValue,float,Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROUGELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float HealthMax;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Attributes")
	float RageMax;

	// UPROPERTY(ReplicatedUsing="")
	// bool bIsAlive;

	//@FIXME:mark as unreliable once we move the 'state' out of scharacter 
	UFUNCTION(NetMulticast,reliable)
	void MulticastHealthChanged(AActor* InstigatorActor,float NewHealth,float Delta);

	UFUNCTION(NetMulticast,Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor,float NewRage,float Delta);
	
public:
	UFUNCTION(BlueprintCallable,Category="Attributes")
	static USAttributeComponent *GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Attributes",meta = (DisplayName="isAlive"))
	static bool isActorAlive(AActor* Actor);
	USAttributeComponent();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyHealthChange(AActor *InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyRageChange(AActor *InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool IsFullHealth();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	float GetHealthMax();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	float GetRageMax();

	UFUNCTION(BlueprintCallable,Category="Attributes")
	float GetCurrentRage();
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;
};
