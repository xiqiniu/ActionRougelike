// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

//利用结构体在改变bIsRunning的时候传递Instigator
USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
public:
	//结构体与类不同,如果不想复制才需要写UPROPERTY(NotReplicated),如果需要复制就加上UPROPERTY()即可
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

/**
 * 
 */
class USActionComponent;
UCLASS(Blueprintable)
class ACTIONROUGELIKE_API USAction : public UObject
{
	GENERATED_BODY()
public:
	void Initialize(USActionComponent* NewActionComp);
	
	UPROPERTY(EditDefaultsOnly,Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintCallable,Category="Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Action")
	void StopAction(AActor *Instigator);
	
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent,Category="Action")
	bool CanStart(AActor* Instigator);
	
	UWorld* GetWorld() const override;
	
protected:
	UPROPERTY(Replicated)
	float TimeStarted;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(Replicated)
	USActionComponent* ActionComp;
	
	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable,Category="Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	bool IsSupportedForNetworking() const override
	{
		return true;
	};
};
