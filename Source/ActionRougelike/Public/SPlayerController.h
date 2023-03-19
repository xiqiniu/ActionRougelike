// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged,APawn*,NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,APlayerState*, NewPlayerState);

/**
 * 
 */
UCLASS()
class ACTIONROUGELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	void SetupInputComponent() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;
	
	//当控制器改变了当前控制的目标时会被调用
	virtual void SetPawn(APawn* InPawn) override;

	//解决控件蓝图中过早调用获取当前玩家的问题
	//解决方案1:改用BeginPlayingState,这样初始化控件的时间就被推后到获取当前玩家能正确返回对应的控制器
	//called when player controller is ready to begin playing, good moment to initialize things like UI which might be too early in BeginPlay
	//(esp. in multiplyaer clients where not all data such as PlayerState may have benn received yet)
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	//解决方案2
	//要在上方声明事件
	void OnRep_PlayerState() override;
	
};
