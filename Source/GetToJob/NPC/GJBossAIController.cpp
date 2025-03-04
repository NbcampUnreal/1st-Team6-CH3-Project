#include "NPC/GJBossAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/GJCharacter.h"
#include "NPC/GJBossNPC.h"

AGJBossAIController::AGJBossAIController()
{
	SetupPerceptionSystem();
}

void AGJBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AGJBossNPC* const npc = Cast<AGJBossNPC>(InPawn))
	{
		if (UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* BlackBoardComponent;
			UseBlackboard(tree->BlackboardAsset, BlackBoardComponent);
			Blackboard = BlackBoardComponent;
			RunBehaviorTree(tree);
		}
	}
}

void AGJBossAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			TEXT("Perception Component")));
		SightConfig->SightRadius = 12000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(10.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 2250.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGJBossAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AGJBossAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<AGJCharacter>(Actor))
	{
		float const DistanceToPlayer = ch->GetDistanceTo(GetPawn()); // 플레이어와 NPC 사이의 거리 계산

		if (DistanceToPlayer <= 10000.f)
		{
			GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed()); // 즉시 추적
		}
	}
	if (auto* const ch = Cast<AGJCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}

void AGJBossAIController::BeginPlay()
{
	Super::BeginPlay();
}
