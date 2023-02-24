// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Try to shoot
    AAIController *Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter *Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Character)
    {
        return EBTNodeResult::Failed;
    }

    Character->Shoot();
    return EBTNodeResult::Succeeded;
}
