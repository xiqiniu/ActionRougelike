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
	//OnRep_LidOpened()ֻ����client�е�bLidOpened�ı�ʱ������,��server����Ҫ�Լ�����
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

	//��bLidOpened�ı�,���е�client����ı�
	DOREPLIFETIME(ASItemChest,bLidOpened);
}
