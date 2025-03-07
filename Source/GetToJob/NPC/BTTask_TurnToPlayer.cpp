#include "NPC/BTTask_TurnToPlayer.h"
#include "NPC/GJ_AIController.h"
#include "NPC/GJBossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/GJCharacter.h"
#include "NPC/GJBossNPC.h"
UBTTask_TurnToPlayer::UBTTask_TurnToPlayer(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Turn To Player");
}

EBTNodeResult::Type UBTTask_TurnToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (auto* const cont = Cast<AGJ_AIController>(OwnerComp.GetAIOwner()))
    {
        if (auto* const npc = Cast<ACharacter>(cont->GetPawn())) // Get the NPC character
        {
            auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

            // Calculate the desired rotation
            FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(npc->GetActorLocation(), PlayerLocation);

            FRotator CurrentRotation = npc->GetActorRotation();
            FRotator TargetRotation = FRotator(CurrentRotation.Pitch, DesiredRotation.Yaw, CurrentRotation.Roll);

            float DeltaTime = OwnerComp.GetWorld()->GetDeltaSeconds();
            FRotator NewRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);

            npc->SetActorRotation(NewRotation);

            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;

        }
    }
    if (auto* const cont = Cast<AGJBossAIController>(OwnerComp.GetAIOwner()))
    {
        if (auto* const npc = Cast<AGJBossNPC>(cont->GetPawn())) // Get the Boss NPC character
        {
            auto const PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());

            // Calculate the desired rotation
            FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(npc->GetActorLocation(), PlayerLocation);

            FRotator CurrentRotation = npc->GetActorRotation();
            FRotator TargetRotation = FRotator(CurrentRotation.Pitch, DesiredRotation.Yaw, CurrentRotation.Roll);

            float DeltaTime = OwnerComp.GetWorld()->GetDeltaSeconds();
            FRotator NewRotation = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);
            npc->SetActorRotation(NewRotation);

            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;

        }
    }

    return EBTNodeResult::Failed;
}
