// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "SPlayerState.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::TogglePauseMenu()
{
	//ʵ��Toggle,�˵��򿪺��ٵ��û�ر�
	if(PauseMenuInstance && PauseMenuInstance -> IsInViewport())
	{
		PauseMenuInstance -> RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this,PauseMenuClass);
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu",IE_Pressed,this,&ASPlayerController::TogglePauseMenu);
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState()
{
	//ʵ������ͼ��
	BlueprintBeginPlayingState();
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateReceived.Broadcast(PlayerState);
}
