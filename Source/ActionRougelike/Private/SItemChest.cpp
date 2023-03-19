// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
	bReplicates = true;
	// SetReplicates(true);
}

void ASItemChest::Interact_Implementation(APawn * InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	//OnRep_LidOpened()只会在client中当bLidOpened改变时被调用,而server则需要自己调用
	OnRep_LidOpened();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	ISGameplayInterface::OnActorLoaded_Implementation();
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened?TargetPitch:0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}


void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//当bLidOpened改变,所有的client都会改变
	DOREPLIFETIME(ASItemChest,bLidOpened);
}
