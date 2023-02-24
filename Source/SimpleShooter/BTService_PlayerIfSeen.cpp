// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_PlayerIfSeen.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerIfSeen::UBTService_PlayerIfSeen()
{
    NodeName = TEXT("Update Player If Seen");
}

void UBTService_PlayerIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController *Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return;
    }

    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    // if player in sight
    if (PlayerPawn && Controller->LineOfSightTo(PlayerPawn))
    {
        // update player
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
    }
    else
    {
        // clear player
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }
}
