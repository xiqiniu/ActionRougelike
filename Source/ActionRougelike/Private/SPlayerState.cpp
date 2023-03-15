// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	//防止加上负数/零
	if(!ensure(Delta>0.0f))
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
