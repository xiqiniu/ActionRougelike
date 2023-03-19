// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged,USActionComponent* ,OwningComp,USAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROUGELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable,Category="Actions")
	void AddAction(TSubclassOf<USAction> ActionClass,AActor *Instigator);

	UFUNCTION(BlueprintCallable,Category="Actions")
	void RemoveAction(USAction* ActionToRemove);

	UFUNCTION(BlueprintCallable,Category="Actions")
	bool StartActionByName(AActor *Instigator,FName ActionName);

	UFUNCTION(BlueprintCallable,Category="Actions")
	bool StopActionByName(AActor *Instigator,FName ActionName);
	
	UFUNCTION(BlueprintCallable)
	USAction* GetAction(TSubclassOf<USAction> ActionClass) const;
	USActionComponent();

protected:
	UFUNCTION(Server,Reliable)
	void ServerStartAction(AActor *Instigator,FName ActionName);

	UFUNCTION(Server,Reliable)
	void ServerStopAction(AActor *Instigator,FName ActionName);
	
	UPROPERTY(EditAnywhere,Category="Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY(BlueprintReadOnly,Replicated)
	TArray<USAction*> Actions;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
};
