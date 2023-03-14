// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(!IsValid(AttachedActor))
	{
		//ºÍRemoveFromViewportÒ»Ñù
		RemoveFromParent();
		UE_LOG(LogTemp,Warning,TEXT("AttachedActor no longer, valid,removing Health Widget."));
		return;
	}
	FVector2D ScreenPosition;
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),AttachedActor->GetActorLocation()+WorldOffset,ScreenPosition))
	{
		float scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition/=scale;
		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
