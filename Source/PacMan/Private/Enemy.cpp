// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/Enemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Public/AIEnemy.h"
#include "../Public/PacManCharacter.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderObj(TEXT("'/Game/StarterContent/Shapes/Shape_Cylinder'"));
	if (CylinderObj.Succeeded()) {
		EnemyBody->SetStaticMesh(CylinderObj.Object);
	}

	EnemyBody->SetWorldScale3D(FVector(0.7f, 0.7f, 1.0f));
	EnemyBody->SetupAttachment(RootComponent);
	EnemyBody->SetRelativeLocation(FVector(0, 0, -50));

	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.0f);

	static ConstructorHelpers::FObjectFinder<UMaterial>VulnerableMat(TEXT("'/Game/Materials/M_Vunleralbe'"));

	AIControllerClass = AAIEnemy::StaticClass();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCollsion);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	//获得当前材质
	DefaultMaterial = EnemyBody->GetMaterial(0);
	SetActorEnableCollision(true);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::SetVulnerable()
{
	GetWorldTimerManager().SetTimer(TimeVulnerable,this,&AEnemy::SetInVulnerable,10.0f,false);
	if (bIsVulnerable) {
		return;
	}
	bIsVulnerable = true;
	EnemyBody->SetMaterial(0, VulnerableMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 50.0f;
}

void AEnemy::SetInVulnerable()
{
	GetWorldTimerManager().ClearTimer(TimeVulnerable);
	bIsVulnerable = false;
	EnemyBody->SetMaterial(0, DefaultMaterial);
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
}

void AEnemy::Killed()
{
	if (bIsDead) {
		return;
	}
	bIsDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	AAIEnemy* AI = Cast<AAIEnemy>(GetController());
	AI->GoHome();
}

void AEnemy::SetMove(bool bMoveIt)
{
	AAIEnemy* AI = Cast<AAIEnemy>(GetController());
	if (bMoveIt) {
		AI->SearchNewPoint();
	}
	else {
		AI->StopMove();
	}

}

void AEnemy::ReArm()
{
	bIsDead = false;
	if (bIsVulnerable) {
		SetInVulnerable();
	}
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	SetMove(true);
}

void AEnemy::OnCollsion(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacManCharacter::StaticClass())) {
		if(bIsVulnerable) {
			Killed();
		}
		else {
			APacManCharacter* PacMan = Cast<APacManCharacter>(OtherActor);
			PacMan->Killed();
		}
	}
}
