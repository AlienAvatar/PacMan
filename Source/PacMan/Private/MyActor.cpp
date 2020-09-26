// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(true);
	myActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test"));
	collsionMesh = CreateDefaultSubobject<USphereComponent>(TEXT("TestCollsion"));
	myActorMesh->SetupAttachment(collsionMesh);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Shape_Cone(TEXT("'/Game/StarterContent/Shapes/Shape_Cone'"));
	if (Shape_Cone.Succeeded()) {
		myActorMesh->SetStaticMesh(Shape_Cone.Object);
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

