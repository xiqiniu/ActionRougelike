// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this,Credits,Credits-OldCredits);
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	//防止加上负数/零
	if(!ensure(Delta>=0.0f))
	{
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this,Credits,Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	//防止减去正数
	if(!ensure(Delta>0.0f))
	{
		return false;
	}
	if(Credits<Delta)
	{
		//点数不足
		return false;
	}
	Credits -= Delta;
	OnCreditsChanged.Broadcast(this,Credits,-Delta);
	return true;
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if(SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if(SaveObject)
	{
		//不能这样直接加载,因为没有触发OnCreditsChanged
		// Credits = SaveObject->Credits;
		AddCredits(SaveObject->Credits);
	}
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState,Credits);
}
                
