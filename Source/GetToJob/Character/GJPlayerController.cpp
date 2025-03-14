#include "GJPlayerController.h"
#include "UI/GJHUD.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "Kismet/GameplayStatics.h"

AGJPlayerController::AGJPlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    SitAction(nullptr),
    FireAction(nullptr),
    ReloadAction(nullptr),
    DropWeaponAction(nullptr),
    ESCAction(nullptr),
    InteractAction(nullptr),
    WeaponSlot1Action(nullptr),
    WeaponSlot2Action(nullptr),
    WeaponSlot3Action(nullptr),
    WeaponSlot4Action(nullptr),
    TestDeathAction(nullptr),
    UseHealingItemAction(nullptr),
    UltimateSkillAction(nullptr),
    AimAction(nullptr),
    HUD(nullptr)
{
}

void AGJPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    HUD = Cast<AGJHUD>(this->GetHUD());
}

void AGJPlayerController::GameOver()
{
    HUD->HideHUD(GJHUDState::MainHUD);
    HUD->DisplayHUD(GJHUDState::GameOver);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void AGJPlayerController::GameClear()
{
    HUD->HideHUD(GJHUDState::MainHUD);
    HUD->DisplayHUD(GJHUDState::GameClear);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void AGJPlayerController::GameClearFail()
{
    HUD->HideHUD(GJHUDState::MainHUD);
    HUD->DisplayHUD(GJHUDState::GameFail);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void AGJPlayerController::Pause()
{
    HUD->DisplayHUD(GJHUDState::PauseHUD);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void AGJPlayerController::QuitGame()
{
    UKismetSystemLibrary::QuitGame
    (
        GetWorld(),
        this,
        EQuitPreference::Quit,
        false
    );
}