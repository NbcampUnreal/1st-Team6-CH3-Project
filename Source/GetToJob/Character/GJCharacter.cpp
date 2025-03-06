#include "Character/GJCharacter.h"
#include "Character/GJPlayerController.h"
#include "GameManager/GJGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/GJBaseGun.h"
#include "Weapon/GJRevolver.h"
#include "Weapon/GJRifle.h"
#include "Weapon/GJRocketLauncher.h"
#include "Weapon/GJMiniGun.h"
#include "Weapon/GJScope.h"
#include "Weapon/GJShotGun.h"
#include "UI/GJHUD.h"
#include "Components/CapsuleComponent.h"
#include "Character/GJHealingItem.h"
#include "GameManager/GJGameInstance.h"
//#include "Components/WidgetComponent.h"
//#include "Components/TextBlock.h"
//#include "Components/ProgressBar.h"

// 생성자
AGJCharacter::AGJCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    // 스프링 암 컴포넌트 생성
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // 스프링 암을 루트 컴포넌트에 부착
    SpringArmComp->SetupAttachment(RootComponent);
    // 캐릭터와 카메라 사이의 거리 기본값 300으로 설정
    SpringArmComp->TargetArmLength = 300.0f;
    // 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
    SpringArmComp->bUsePawnControlRotation = true;

    // 카메라 컴포넌트 생성
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // 스프링 암의 소켓 위치에 카메라를 부착
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    // 카메라는 스프링 암의 회전을 따르므로 PawnControlRotation은 꺼둠
    CameraComp->bUsePawnControlRotation = false;

    /*HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
      HPWidget->SetupAttachment(GetMesh());
      HPWidget->SetWidgetSpace(EWidgetSpace::Screen);*/

    // 속도 설정
    NormalSpeed = 400.0f; // 걷기 속도
    SprintSpeedMultiplier = 1.5f; // 스프린트 속도 배율
    CrouchSpeed = 200.0f; // 앉기 속도
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier; // 스프린트 속도 적용
    BackwardSpeedMultiplier = 0.5f; // 뒤로 가는 속도 배율

    // 마우스 감도 설정
    LookSensitivity = 1.0f; // 감도 조절을 위한 변수

    //// 체력 설정
    //MaxHealth = 100.0f; // 최대체력

    // 기본 무기 설정(일단 캐릭터에서 설정하고 후에 인벤토리에서 재구현?)
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // 캐릭터가 Crouch 기능을 사용할 수 있도록 설정
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    
    // 현재 소지 총 초기화
    CurrentGun = nullptr;
    SetHealth(100.f);
    SetMaxHealth(100.f);

    // 인벤토리 연결
    InventoryComponent = CreateDefaultSubobject<UGJInventoryComponent>(TEXT("InventoryComponent"));

    // 초기 무기 상태 초기화
    CurrentWeaponType = EWeaponType::None;
    
    PrimaryActorTick.bCanEverTick = true;

    DebuffComponent = CreateDefaultSubobject<UGJDebuffComponent>(TEXT("DebuffComponent"));

    // 생성시 플레이어 컨트롤러 연결
    GJController = Cast<AGJPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    bIsInvincible = false; // 기본적으로 무적 상태 아님
}

// 궁극기 발동 (T키)
void AGJCharacter::ActivateUltimateWeapon()
{
    UE_LOG(LogTemp, Warning, TEXT("ActivateUltimateWeapon() Called!"));

    // 미니건 체크
    if (!MiniGun)
    {
        UE_LOG(LogTemp, Error, TEXT("MiniGun is NULL! Check if it's properly initialized."));
        return;
    }

    // 미니건이 활성화 가능한 상태인지 확인
    if (!MiniGun->bUltraIsReady || MiniGun->GetCurrentGauge() < MiniGun->GetMaxGauge())
    {
        UE_LOG(LogTemp, Warning, TEXT("MiniGun activation failed: Not enough gauge!"));
        return;
    }

    // 미니건 활성화
    UE_LOG(LogTemp, Warning, TEXT("MiniGun Found! Activating..."));
    MiniGun->ActivateMiniGun();
}

void AGJCharacter::ModifyHealth(float Amount)
{
    // 이미 죽었으면 체력 조정 X
    if (bIsDead) return;

    // 체력 변경 (최대 체력을 초과하지 않도록 제한)
    float NewHealth = FMath::Clamp(GetHealth() + Amount, 0.0f, GetMaxHealth());
    SetHealth(NewHealth);

    // 체력이 0 이하이면 사망 처리 (단, 중복 호출 방지)
    if (GetHealth() <= 0 && !bIsDead)
    {
        bIsDead = true; // 사망 상태로 설정
        UE_LOG(LogTemp, Warning, TEXT("Character has died!"));
        OnDeath();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Character Health Modified: %f / %f"), GetHealth(), GetMaxHealth());
    }
}

void AGJCharacter::EnableGameInput()
{
    if (GJController)
    {
        FInputModeGameOnly InputMode;
        GJController->SetInputMode(InputMode);
        GJController->bShowMouseCursor = false;

        UE_LOG(LogTemp, Warning, TEXT("Game Input Mode Restored!"));
    }
}


void AGJCharacter::BeginPlay()
{
    Super::BeginPlay();

    UGJGameInstance* GameInstance = Cast<UGJGameInstance>(GetGameInstance());
    if (GameInstance)
    {
        GameInstance->LoadCharacterState(this);
    }

    // 컨트롤러 재 확인
    if (!GJController)
    {
        GJController = Cast<AGJPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    }

    bIsDead = false;

    EnableGameInput();

    if (MiniGunClass)
    {
        MiniGun = GetWorld()->SpawnActor<AGJMiniGun>(MiniGunClass);
        if (MiniGun)
        {
            MiniGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MiniGun->GunSocketName);
            MiniGun->SetOwner(this);

            /*MiniGun->GunMesh->SetVisibility(false);*/
            MiniGun->GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

            UE_LOG(LogTemp, Warning, TEXT("MiniGun Initialized at Game Start"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MiniGun Spawn FAILED!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MiniGunClass is NULL!"));
    }
}


// 캐릭터 이동속도 저장 및 유지하는 함수
void AGJCharacter::SetSpeed(float NewSpeedMultiplier)
{
    float SpeedMultiplier = FMath::Clamp(NewSpeedMultiplier, 0.1f, 1.0f); // 속도가 너무 작아지지 않도록 제한

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SpeedMultiplier;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier * SpeedMultiplier;
    GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed * SpeedMultiplier;

    UE_LOG(LogTemp, Warning, TEXT("Speed Updated -> Walk: %f | Sprint: %f"),
        GetCharacterMovement()->MaxWalkSpeed, SprintSpeed);
}

// 디버프 적용 함수
void AGJCharacter::ApplyDebuff(const FDebuffEffect& Debuff)
{
    if (DebuffComponent)
    {
        DebuffComponent->ApplyDebuff(Debuff);
    }
}

void AGJCharacter::UseHealingItem()
{
    if (InventoryComponent)
    {
        InventoryComponent->UseHealingItem();
    }
}

void AGJCharacter::Interact()
{
    // 1️ 무기 먼저 줍기
    if (IsValid(InteractableWeapon) && InventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("상호작용 키로 무기 줍기 시도: %s"), *InteractableWeapon->GetName());

        AGJBaseGun* TempWeapon = InteractableWeapon; // 임시 저장

        if (TempWeapon)
        {
            TempWeapon->Pickup(this); // 무기 줍기
            InventoryComponent->AddWeapon(TempWeapon); // 인벤토리에 추가
            EquipWeapon(TempWeapon); // 줍자마자 장착

            // 로그 추가해서 무기가 정상적으로 장착되는지 확인
            UE_LOG(LogTemp, Warning, TEXT("Weapon Equipped: %s"), *TempWeapon->GetName());
        }

        // 여기에서 초기화 (너무 일찍 하지 않도록!)
        InteractableWeapon = nullptr;
        return; // 무기 줍는 경우, 힐링 아이템 줍기 로직은 실행하지 않음
    }

    // 2️ 힐링 아이템 줍기
    if (IsValid(InteractableHealingItem) && InventoryComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("힐링 아이템 인벤토리에 추가"));

        AGJHealingItem* TempHealingItem = InteractableHealingItem; // 임시 저장
        InteractableHealingItem = nullptr; // 초기화

        if (TempHealingItem)
        {
            TempHealingItem->Pickup(this);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("상호작용할 힐링 아이템 없음"));
    }
}

void AGJCharacter::FireWeapon()
{
    if (!CurrentGun)
    {
        UE_LOG(LogTemp, Error, TEXT("FireWeapon Failed: CurrentGun is NULL!"));
        return;
    }

    CurrentGun->Fire(); // 발싸

    //애니메이션 추가 currentGunt->fire() 밑에 두었습니다.
    if (GJController)
    {
        GJController->HUD->ShowFireAnim();
    }
}

void AGJCharacter::ReloadWeapon()
{
    // 무기 변수 또는 필수 포인터가 `nullptr`인지 확인
    if (!CurrentGun)
    {
        UE_LOG(LogTemp, Error, TEXT("ReloadWeapon Failed: CurrentWeapon is nullptr!"));
        return;
    }

    CurrentGun->Reload();
}

//void AGJCharacter::DropWeapon()
//{
//    if (CurrentGun)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("Weapon Dropped!"));
//
//        // 인벤토리에서 무기 제거
//        if (InventoryComponent)
//        {
//            InventoryComponent->RemoveWeapon(CurrentGun);
//        }
//
//        // 무기 비활성화 및 던지기 처리
//        CurrentGun->ThrowAway();
//
//        // 무기 애니메이션 상태 초기화
//        CurrentWeaponType = EWeaponType::None;
//
//        // 현재 무기 제거
//        CurrentGun = nullptr;
//
//        // 애니메이션 상태 업데이트 로그
//        UE_LOG(LogTemp, Log, TEXT("Weapon successfully dropped and removed from inventory. Weapon state set to NoWeapon."));
//    }
//}

void AGJCharacter::EquipWeapon(AGJBaseGun* NewWeapon)
{
    if (!NewWeapon)
    {
        UE_LOG(LogTemp, Error, TEXT("EquipWeapon Failed: Weapon is nullptr!"));
        return;
    }

    if (CurrentGun == NewWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon is already equipped."));
        return;
    }

    // 기존 무기 해제 및 숨김 처리
    if (CurrentGun)
    {
        CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentGun->SetActorHiddenInGame(true);
        CurrentGun->SetActorEnableCollision(false);
        if (CurrentGun->Attachments.Num() > 0)
        {
            TArray<USkeletalMeshComponent*> MeshComponents;
            CurrentGun->Attachments[0]->GetComponents<USkeletalMeshComponent>(MeshComponents);

            for (USkeletalMeshComponent* MeshComp : MeshComponents)
            {
                if (MeshComp)
                {
                    MeshComp->SetVisibility(false);
                }
            }
        }
        
    }

    // 새 무기 장착
    CurrentGun = NewWeapon;
    FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
    CurrentGun->AttachToComponent(GetMesh(), AttachRules, CurrentGun->GunSocketName);

    // 무기 다시 보이게 활성화
    CurrentGun->SetActorHiddenInGame(false);
    CurrentGun->SetActorEnableCollision(true);
    CurrentGun->SetOwner(this);
    if (CurrentGun->Attachments.Num() > 0)
    {
        TArray<USkeletalMeshComponent*> MeshComponents;
        CurrentGun->Attachments[0]->GetComponents<USkeletalMeshComponent>(MeshComponents);

        for (USkeletalMeshComponent* MeshComp : MeshComponents)
        {
            if (MeshComp)
            {
                MeshComp->SetVisibility(true);
            }
        }
    }

    // 무기 상태 업데이트
    UpdateWeaponState(CurrentGun);

    // 로그 추가해서 장착된 무기가 무엇인지 확인
    UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon: %s"), *CurrentGun->GetName());
    UE_LOG(LogTemp, Warning, TEXT("CurrentWeaponType: %d"), static_cast<int32>(CurrentWeaponType));
}

void AGJCharacter::UnequipCurrentWeapon()
{
    if (CurrentGun)
    {
        CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentGun = nullptr;
        UE_LOG(LogTemp, Log, TEXT("Current Weapon Unequipped"));
    }
}

void AGJCharacter::EquipWeaponFromInventory(int32 SlotIndex)
{
    // 인벤토리 및 슬롯 유효성 검사
    if (InventoryComponent && InventoryComponent->WeaponSlots.IsValidIndex(SlotIndex))
    {
        // 슬롯에 무기가 실제로 존재하는 경우
        if (InventoryComponent->WeaponSlots[SlotIndex])
        {
            InventoryComponent->EquipWeaponFromSlot(SlotIndex);
            CurrentGun->SwapAttachmentsWithGun(CurrentGun, InventoryComponent->EquipWeaponFromSlot(SlotIndex));
            UE_LOG(LogTemp, Log, TEXT("Equipped weapon from slot %d."), SlotIndex);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Slot %d is valid but empty."), SlotIndex);
        }
    }
    else
    {
        // 슬롯 유효성 검사에서 실패한 경우
        UE_LOG(LogTemp, Warning, TEXT("Invalid Slot: %d. Total slots available: %d"),
            SlotIndex, InventoryComponent->WeaponSlots.Num());

        // 무기 장착 시도 방지
        return;
    }
}

void AGJCharacter::UpdateWeaponState(AGJBaseGun* NewWeapon)
{
    PreviousWeaponType = CurrentWeaponType;

    if (!NewWeapon)
    {
        CurrentWeaponType = EWeaponType::None;
        return;
    }

    if (Cast<AGJRevolver>(NewWeapon))
    {
        CurrentWeaponType = EWeaponType::Revolver;
    }
    else if (Cast<AGJRifle>(NewWeapon))
    {
        CurrentWeaponType = EWeaponType::Rifle;
    }
    else if (Cast<AGJRocketLauncher>(NewWeapon))
    {
        CurrentWeaponType = EWeaponType::RocketLauncher;
    }
    else if (Cast<AGJMiniGun>(NewWeapon)) 
    {
        CurrentWeaponType = EWeaponType::MiniGun;
    }
    else if (Cast<AGJShotGun>(NewWeapon))
    {
        CurrentWeaponType = EWeaponType::ShotGun;
    }
    else
    {
        CurrentWeaponType = EWeaponType::None;
    }

    UE_LOG(LogTemp, Log, TEXT("Weapon type updated: %d -> %d"),
        static_cast<uint8>(PreviousWeaponType),
        static_cast<uint8>(CurrentWeaponType));
}

float AGJCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamgeCauser)
{

    // 무적 상태이면 데미지를 받지 않음
    if (bIsInvincible)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is invincible! No damage taken."));
        return 0.0f;
    }

    float ActualDamage = Super::TakeDamage(DamageAmount,
        DamageEvent,
        EventInstigator,
        DamgeCauser);
    SetHealth(FMath::Clamp(GetHealth() - DamageAmount, 0.0f, GetMaxHealth()));

    if (GJController)
    {
        GJController->HUD->ShowHitEffect();
    }

    if (auto const Player = Cast<AGJCharacter>(this))
    {
        if (GetHealth() <= 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Dead"));
            OnDeath();
            bIsDead = true;  // 중복 실행 방지
        }
    }
    return ActualDamage;
}

void AGJCharacter::StopFireWeapon()
{
    bIsFiring = false;
}

void AGJCharacter::Tick(float Deltatime)
{
    Super::Tick(Deltatime);
    float Speed = GetVelocity().Size();
    if (FMath::Abs(Speed - LastSpeed) > 5.0f)
    {
        LastSpeed = Speed;
        float NewSpread = FMath::GetMappedRangeValueClamped(
            FVector2D(0.0f, 450.0f),
            FVector2D(5.0f, 70.0f),
            Speed
        );

        if (GJController)
        {
            GJController->HUD->UpdateCrosshairSize(NewSpread);
        }
    }
}

void AGJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (AGJPlayerController* PlayerController = Cast<AGJPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // 무브먼트(WASD)
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                // 점프 (Space)
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::StartJump
                );

                // 점프 멈추기 (떼기)
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AGJCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                // 시선 (마우스 움직임)
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // 스프린트 (Shift)
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::StartSprint
                );
                // 다시 걷기(떼기)
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &AGJCharacter::StopSprint
                );
            }

            if (PlayerController->SitAction)
            {
                // 앉기 (ctrl)
                EnhancedInput->BindAction(
                    PlayerController->SitAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::StartSit
                );
                // 앉기 멈추고 일어서기 (떼기)
                EnhancedInput->BindAction(
                    PlayerController->SitAction,
                    ETriggerEvent::Completed,
                    this,
                    &AGJCharacter::StopSit
                );
            }

            if (PlayerController->FireAction)
            {
                // 발사 (좌클릭)
                EnhancedInput->BindAction(
                    PlayerController->FireAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::FireWeapon
                );
            }

            if (PlayerController->ReloadAction)
            {
                // 재장전 (R)
                EnhancedInput->BindAction(
                    PlayerController->ReloadAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::ReloadWeapon
                );
            }

            //if (PlayerController->ESCAction)
            //{
            //    // 메인메뉴 (ESC)
            //    EnhancedInput->BindAction(
            //        PlayerController->ESCAction, 
            //        ETriggerEvent::Triggered, 
            //        PlayerController,
            //        &AGJPlayerController::OpenMainMenu);
            //}

            //if (PlayerController->DropWeaponAction)
            //{
            //    // 무기 드롭(G)
            //    EnhancedInput->BindAction(
            //        PlayerController->DropWeaponAction, 
            //        ETriggerEvent::Triggered, 
            //        this, 
            //        &AGJCharacter::DropWeapon);
            //}

            if (PlayerController->InteractAction)
            {
                // E키 - 상호작용
                EnhancedInput->BindAction(
                    PlayerController->InteractAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::Interact
                );
            }

            // 무기 선택 (1, 2, 3)
            if (PlayerController->WeaponSlot1Action)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponSlot1Action,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::EquipWeaponFromInventory, 0
                );
            }

            if (PlayerController->WeaponSlot2Action)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponSlot2Action,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::EquipWeaponFromInventory, 1
                );
            }

            if (PlayerController->WeaponSlot3Action)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponSlot3Action,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::EquipWeaponFromInventory, 2
                );
            }

            if (PlayerController->WeaponSlot4Action)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponSlot4Action,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::EquipWeaponFromInventory, 3
                );
            }

            if (PlayerController->TestDeathAction)
            {
                // 테스트용 죽음 트리거 (예: K 키)
                EnhancedInput->BindAction(
                    PlayerController->TestDeathAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::TriggerDeathTest
                );
            }

            // 힐링액션
            if (PlayerController->UseHealingItemAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->UseHealingItemAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::UseHealingItem
                );
            }

            // T 키 - 궁극기 발동
            EnhancedInput->BindAction(
                PlayerController->UltimateSkillAction,
                ETriggerEvent::Started,
                this,
                &AGJCharacter::ActivateUltimateWeapon
            );
            // 우클릭(조준) - 줌인
            EnhancedInput->BindAction(PlayerController->AimAction, ETriggerEvent::Started, this, &AGJCharacter::StartAiming);
            // 우클릭 해제 - 줌아웃
            EnhancedInput->BindAction(PlayerController->AimAction, ETriggerEvent::Completed, this, &AGJCharacter::StopAiming);
        }
    }
}

void AGJCharacter::Move(const FInputActionValue& value)
{
    // 컨트롤러가 있어야 방향 계산이 가능
    if (!Controller) return;

    // Value는 Axis2D로 설정된 IA_Move의 입력값(WASD)
    const FVector2D MoveInput = value.Get<FVector2D>();

    // 기본 이동 속도
    float MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // 캐릭터가 바라보는 방향(정면)으로 X축 이동
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // 캐릭터의 오른쪽 방향으로 Y축 이동
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // 후진 시 속도를 줄이기 (예: 기본 속도의 50%)
        float SpeedMultiplier = (MoveInput.X < 0) ? 0.5f : 1.0f;
        AddMovementInput(GetActorForwardVector(), MoveInput.X * BackwardSpeedMultiplier);
    }
}

void AGJCharacter::StartJump(const FInputActionValue& value)
{
    // 앉은 상태에서는 점프하지 않도록 조건 추가
    if (!bCanJump || GetCharacterMovement()->IsCrouching())
    {
        return;
    }

    Jump(); // 점프 실행
    bCanJump = false; // 점프 비활성화 (쿨다운 시작)
}

void AGJCharacter::StopJump(const FInputActionValue& value)
{
    // StopJumping 함수도 Character가 기본 제공
    StopJumping(); // 마찬가지로 바로 실행
}

void AGJCharacter::Landed(const FHitResult& Hit) // 착지 시점 감지
{
    Super::Landed(Hit);
    // 착지 후 점프 쿨다운 시작
    GetWorldTimerManager().SetTimer(JumpCooldownTimer, this, &AGJCharacter::ResetJump, JumpCooldownTime, false);
}

void AGJCharacter::ResetJump() // 점프 재활성화 함수
{
    bCanJump = true;
}

void AGJCharacter::Look(const FInputActionValue& value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = value.Get<FVector2D>();

    // X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
    // 좌우 회전
    AddControllerYawInput(LookInput.X * LookSensitivity);
    // 상하 회전
    AddControllerPitchInput(LookInput.Y * LookSensitivity);
}

void AGJCharacter::StartSprint(const FInputActionValue& value)
{
    // Shift 키를 누른 순간 이 함수가 호출
    // 스프린트 속도를 적용
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AGJCharacter::StopSprint(const FInputActionValue& value)
{
    // Shift 키를 뗀 순간 이 함수가 호출
    // 평상시 속도로 복귀
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void AGJCharacter::StartSit(const FInputActionValue& value)
{
    if (GetCharacterMovement()->NavAgentProps.bCanCrouch) // 예외 처리
    {
        Crouch(); // 앉기
        GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;// 앉은 상태 속도 조절
    }
}

void AGJCharacter::StopSit(const FInputActionValue& value)
{
    if (GetCharacterMovement()->NavAgentProps.bCanCrouch)
    {
        UnCrouch(); // 앉기 해제
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // 원래 속도로 복귀

        // 앉기에서 일어났을 때 점프 가능하게 재설정
        bCanJump = true;
    }
}

void AGJCharacter::StartAiming()
{
    // CurrentGun이 nullptr인지 먼저 확인
    if (!CurrentGun)
    {
        UE_LOG(LogTemp, Error, TEXT("StartAiming Failed: CurrentGun is NULL!"));
        return;
    }

    // CurrentGun->Attachments가 유효한지 확인
    if (CurrentGun->Attachments.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartAiming Skipped: No attachments found on current weapon."));
        return;
    }

    // bHasScope가 true인지 확인
    if (!CurrentGun->bHasScope)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartAiming Skipped: CurrentGun does not have a scope!"));
        return;
    }

    for (AGJBaseGunAttachment* Attachment : CurrentGun->Attachments)
    {
        if (!Attachment)
        {
            UE_LOG(LogTemp, Error, TEXT("StartAiming Failed: Null attachment found in weapon attachments!"));
            continue;
        }

        if (AGJScope* Scope = Cast<AGJScope>(Attachment))
        {
            Scope->EnableScopeView();
            UE_LOG(LogTemp, Log, TEXT("StartAiming: Scope activated successfully."));
        }
    }
}

void AGJCharacter::StopAiming()
{
    // CurrentGun이 nullptr인지 먼저 확인
    if (!CurrentGun)
    {
        UE_LOG(LogTemp, Error, TEXT("StopAiming Failed: CurrentGun is NULL!"));
        return;
    }

    // CurrentGun->Attachments가 유효한지 확인
    if (CurrentGun->Attachments.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("StopAiming Skipped: No attachments found on current weapon."));
        return;
    }

    // bHasScope가 true인지 확인
    if (!CurrentGun->bHasScope)
    {
        UE_LOG(LogTemp, Warning, TEXT("StopAiming Skipped: CurrentGun does not have a scope!"));
        return;
    }

    for (AGJBaseGunAttachment* Attachment : CurrentGun->Attachments)
    {
        if (!Attachment)
        {
            UE_LOG(LogTemp, Error, TEXT("StopAiming Failed: Null attachment found in weapon attachments!"));
            continue;
        }

        if (AGJScope* Scope = Cast<AGJScope>(Attachment))
        {
            Scope->DisableScopeView();
            UE_LOG(LogTemp, Log, TEXT("StopAiming: Scope deactivated successfully."));
        }
    }
}

void AGJCharacter::OnDeath()
{
    if (bIsDead) return; // 이미 죽었다면 다시 실행되지 않음

    bIsDead = true; // 사망 상태 설정

    // 모든 디버프 제거 (출혈 포함)
    if (DebuffComponent)
    {
        DebuffComponent->RemoveAllDebuffs();
    }

    // 입력 비활성화
    //if (GetController())
    //{
    //    GetController()->DisableInput(nullptr);
    //    GetController()->UnPossess();
    //}

    UE_LOG(LogTemp, Error, TEXT("Player has died! Game Over"));

    // 사망 사운드 재생 (한 번만 실행되도록 보장)
    if (DeathSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
    }

    if (GJController)
    {
        GJController->GameOver();
    }

    // 레그돌 활성화
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    GetMesh()->SetSimulatePhysics(true);

    // 캡슐 콜라이더 비활성화
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    // 일정 시간 후 캐릭터 제거
    SetLifeSpan(30.0f);
}

// 테스트용 죽이기 함수
void AGJCharacter::TriggerDeathTest()
{
    UE_LOG(LogTemp, Warning, TEXT("Test Death Triggered!"));
    OnDeath();
}