// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemy.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void AAIEnemy::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	Bot = Cast<AEnemy>(InPawn);
	HomeLocation = Bot->GetActorLocation();
	SearchNewPoint();
	GameMode = Cast<APacManGameModeBase>(UGameplayStatics::GetGameMode(this));
}

void AAIEnemy::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!Bot->bIsDead && GameMode->GetCurrentState() != EGameState::EPause) {
		SearchNewPoint();
	}
}

void AAIEnemy::SearchNewPoint()
{

	//UNavigationSystemV1* NavMesh = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	UNavigationSystemV1 * NavMesh = UNavigationSystemV1::GetCurrent(this);

	if (NavMesh) {
		//²éÕÒ°ë¾¶
		const float SearchRadius = 10000.0f;
		FNavLocation RandomPt;

		bool bIsFound = NavMesh->GetRandomPointInNavigableRadius(Bot->GetActorLocation(), SearchRadius, RandomPt);
		if (bIsFound) {
			MoveToLocation(RandomPt);
		}
	}
}

void AAIEnemy::StopMove()
{
	MoveToLocation(Bot->GetActorLocation());
}

void AAIEnemy::ReArm()
{
	GetWorldTimerManager().ClearTimer(DeadTime);
	Bot->ReArm();
}

void AAIEnemy::GoHome()
{
	MoveToLocation(HomeLocation);
	GetWorldTimerManager().SetTimer(DeadTime, this, &AAIEnemy::ReArm, 2.0f, false);
}
