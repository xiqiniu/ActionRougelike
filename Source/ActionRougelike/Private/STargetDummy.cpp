// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"


// Sets default values
ASTargetDummy::ASTargetDummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//<要生成的类>("编辑器里的名字")
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent=MeshComp;
	
	AttributeCommp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	//trigged when health changed
	AttributeCommp->OnHealthChanged.AddDynamic(this,&ASTargetDummy::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit",GetWorld()->TimeSeconds);
	}
}

// Called every frame
void ASTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

