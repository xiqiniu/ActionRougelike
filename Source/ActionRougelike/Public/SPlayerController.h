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
	
	//���������ı��˵�ǰ���Ƶ�Ŀ��ʱ�ᱻ����
	virtual void SetPawn(APawn* InPawn) override;

	//����ؼ���ͼ�й�����û�ȡ��ǰ��ҵ�����
	//�������1:����BeginPlayingState,������ʼ���ؼ���ʱ��ͱ��ƺ󵽻�ȡ��ǰ�������ȷ���ض�Ӧ�Ŀ�����
	//called when player controller is ready to begin playing, good moment to initialize things like UI which might be too early in BeginPlay
	//(esp. in multiplyaer clients where not all data such as PlayerState may have benn received yet)
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	//�������2
	//Ҫ���Ϸ������¼�
	void OnRep_PlayerState() override;
	
};
