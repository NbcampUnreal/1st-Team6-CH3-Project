#include "GJPlayerController.h"
#include "UI/GJHUD.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System�� Local Player Subsystem�� ����ϱ� ���� ����
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

    // ���� PlayerController�� ����� Local Player ��ü�� ������    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player���� EnhancedInputLocalPlayerSubsystem�� ȹ��
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
                // �켱����(Priority)�� 0�� ���� ���� �켱����
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