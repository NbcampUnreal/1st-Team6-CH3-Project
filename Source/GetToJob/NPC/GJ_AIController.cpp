#include "NPC/GJ_AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/GJCharacter.h"
#include "GJNPC.h"

AGJ_AIController::AGJ_AIController(FObjectInitializer const& FObjectInitializer)
{
	SetupPerceptionSystem();
}

void AGJ_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AGJNPC* const npc = Cast<AGJNPC>(InPawn))
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

void AGJ_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			TEXT("Perception Component")));
		SightConfig->SightRadius = 700.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGJ_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AGJ_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const ch = Cast<AGJCharacter>(Actor))
	{
		float const DistanceToPlayer = ch->GetDistanceTo(GetPawn()); // 플레이어와 NPC 사이의 거리 계산

		if (DistanceToPlayer <= 500.f) // 500 거리 이내에 있을 경우
		{
			GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed()); // 즉시 추적
		}
	}
	if (auto* const ch = Cast<AGJCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}

void AGJ_AIController::BeginPlay()
{
	Super::BeginPlay();
}
