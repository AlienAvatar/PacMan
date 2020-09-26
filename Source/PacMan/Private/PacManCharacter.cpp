// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/PacManCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/Collectable.h"
#include "Components/CapsuleComponent.h"
#include "EngineUtils.h"

// Sets default values
APacManCharacter::APacManCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacManCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<APacManGameModeBase>(UGameplayStatics::GetGameMode(this));
	Lives = 3;
	StartPoint = GetActorLocation();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacManCharacter::OnCollsion);
	for (TActorIterator<ACollectable> CollectableItr(GetWorld()); CollectableItr; ++CollectableItr) {
		CollectableToEat++;
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Collectable is %d"), CollectableToEat);
	UE_LOG(LogTemp, Warning, TEXT("Start Point is %s"), *StartPoint.ToString());
}

// Called every frame
void APacManCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APacManCharacter::MoveXAxis);
	PlayerInputComponent->BindAxis("MoveY", this, &APacManCharacter::MoveYAxis);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &APacManCharacter::ReStart);
	PlayerInputComponent->BindAction("NewGame", IE_Pressed, this, &APacManCharacter::NewGame);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APacManCharacter::Pause);
}

void APacManCharacter::Killed() {
	if (--Lives == 0) {
		GameMode->SetCurrentState(EGameState::EGameOver);
	}
	else {
		SetActorLocation(StartPoint);
	}
}

void APacManCharacter::MoveXAxis(float AxisValue) {
	if (GameMode->GetCurrentState() == EGameState::EPlaying) {
		CurrentVelocity.X = AxisValue;
		AddMovementInput(CurrentVelocity);
	}
}

void APacManCharacter::MoveYAxis(float AxisValue) {
	if (GameMode->GetCurrentState() == EGameState::EPlaying) {
		CurrentVelocity.Y = AxisValue;
		AddMovementInput(CurrentVelocity);
	}
}

void APacManCharacter::ReStart()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestratLevel"));
}

void APacManCharacter::NewGame()
{
	if (GameMode->GetCurrentState() == EGameState::EMenu) {
		GameMode->SetCurrentState(EGameState::EPlaying);
	}
}

void APacManCharacter::Pause()
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying) {
		GameMode->SetCurrentState(EGameState::EPause);
	}else if (GameMode->GetCurrentState() == EGameState::EPause) {
		GameMode->SetCurrentState(EGameState::EPlaying);
	}
}

void APacManCharacter::OnCollsion(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameMode->GetCurrentState() == EGameState::EPlaying) {
		ACollectable* collectable = Cast<ACollectable>(OtherActor);
		if (collectable->bIsSuperCollectable) {
			GameMode->SetVulnerableEnemy();
		}

		if (OtherActor->IsA(ACollectable::StaticClass())) {
			OtherActor->Destroy();
			if (--CollectableToEat == 0) {
				GameMode->SetCurrentState(EGameState::EWin);
			}
			UE_LOG(LogTemp, Warning, TEXT("Remain Collectable is %d"), CollectableToEat);
		}
	}
}

