#include "GJPlayerController.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함

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
    TestDeathAction(nullptr)
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
}

void AGJPlayerController::OpenMainMenu()
{
    //if (!MainMenuWidget)
    //{
    //    if (MainMenuClass)
    //    {
    //        MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuClass);
    //        if (MainMenuWidget)
    //        {
    //            MainMenuWidget->AddToViewport();
    //            SetShowMouseCursor(true);
    //            SetInputMode(FInputModeUIOnly());
    //        }
    //    }
    //}
    //else
    //{
    //    MainMenuWidget->RemoveFromParent();
    //    MainMenuWidget = nullptr;
    //    SetShowMouseCursor(false);
    //    SetInputMode(FInputModeGameOnly());
    //}
}

void AGJPlayerController::ShowGameHUD()
{
}

void AGJPlayerController::StartGame()
{
    /*if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        SpartaGameInstance->CurrentLevelIndex = 0;
        SpartaGameInstance->TotalScore = 0;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));*/
}