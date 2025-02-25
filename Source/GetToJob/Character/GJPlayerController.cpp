#include "GJPlayerController.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System�� Local Player Subsystem�� ����ϱ� ���� ����

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