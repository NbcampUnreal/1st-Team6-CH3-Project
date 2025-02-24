#include "Character/GJCharacter.h"
#include "GJPlayerController.h"
//#include "GJGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/GJBaseGun.h"
#include "Weapon/GJRevolver.h"
#include "Weapon/GJRifle.h"
#include "Weapon/GJRocketLauncher.h"
#include "Components/CapsuleComponent.h"
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
    NormalSpeed = 600.0f; // 걷기 속도
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
}

void AGJCharacter::Interact()
{
    // 간단한 로그 출력
    UE_LOG(LogTemp, Log, TEXT("Interact Key Pressed"));

    // 추후 오브젝트 상호작용 로직 구현 가능
}

void AGJCharacter::FireWeapon()
{
    if (!CurrentGun)
    {
        UE_LOG(LogTemp, Error, TEXT("FireWeapon Failed: CurrentGun is NULL!"));
        return;
    }

    CurrentGun->Fire(); // 발싸
}

void AGJCharacter::ReloadWeapon()
{
    // 🔍 무기 변수 또는 필수 포인터가 `nullptr`인지 확인
    if (!CurrentGun)
    {
        UE_LOG(LogTemp, Error, TEXT("ReloadWeapon Failed: CurrentWeapon is nullptr!"));
        return;
    }

    CurrentGun->Reload();
}

void AGJCharacter::DropWeapon()
{
    if (CurrentGun)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon Dropped!"));
        CurrentGun->ThrowAway();
    }
}

float AGJCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamgeCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount,
        DamageEvent,
        EventInstigator,
        DamgeCauser);
    SetHealth(FMath::Clamp(GetHealth() - DamageAmount, 0.0f, GetMaxHealth()));
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

void AGJCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 무기가 없으면 블루프린트 무기 생성
    /*if (WeaponSlots.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No weapons found! Initializing default BP weapons."));

        if (BP_GJRevolver)
        {
            AGJRevolver* Revolver = GetWorld()->SpawnActor<AGJRevolver>(BP_GJRevolver);
            if (Revolver)
            {
                AddWeaponToSlot(Revolver);
                UE_LOG(LogTemp, Log, TEXT("Revolver successfully spawned and added to slot."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Revolver!"));
            }
        }

        if (BP_GJRifle)
        {
            AGJRifle* Rifle = GetWorld()->SpawnActor<AGJRifle>(BP_GJRifle);
            if (Rifle)
            {
                AddWeaponToSlot(Rifle);
                UE_LOG(LogTemp, Log, TEXT("Rifle successfully spawned and added to slot."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Rifle!"));
            }
        }

        if (BP_GJRocketLauncher)
        {
            AGJRocketLauncher* RocketLauncher = GetWorld()->SpawnActor<AGJRocketLauncher>(BP_GJRocketLauncher);
            if (RocketLauncher)
            {
                AddWeaponToSlot(RocketLauncher);
                UE_LOG(LogTemp, Log, TEXT("Rocket Launcher successfully spawned and added to slot."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn Rocket Launcher!"));
            }
        }
    }*/


    //// 첫 번째 무기 장착
    //if (WeaponSlots.IsValidIndex(0))
    //{
    //    EquipWeaponFromSlot(0);
    //}
}


void AGJCharacter::AddWeaponToSlot(AGJBaseGun* NewWeapon)
{
    if (!NewWeapon)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot add weapon: Invalid weapon"));
        return;
    }

    // 슬롯이 가득 찼다면 오래된 무기를 제거하고 새 무기 추가
    if (WeaponSlots.Num() >= MaxWeaponSlots)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon slots are full! Removing oldest weapon."));

        AGJBaseGun* OldWeapon = WeaponSlots[0];
        OldWeapon->ThrowAway();
        WeaponSlots.RemoveAt(0);
    }

    // 슬롯에 무기 추가
    WeaponSlots.Add(NewWeapon);
    UE_LOG(LogTemp, Log, TEXT("Weapon added to slot %d: %s"), WeaponSlots.Num(), *NewWeapon->GetName());

    // 장착된 무기가 없다면 첫 번째 무기로 자동 장착
    if (!CurrentGun)
    {
        EquipWeaponFromSlot(WeaponSlots.Num() - 1);
    }
}

void AGJCharacter::EquipWeaponFromSlot(int32 SlotIndex)
{
    if (!WeaponSlots.IsValidIndex(SlotIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid weapon slot index: %d"), SlotIndex);
        return;
    }

    AGJBaseGun* NewWeapon = WeaponSlots[SlotIndex];

    if (!NewWeapon)
    {
        UE_LOG(LogTemp, Error, TEXT("No weapon in slot %d"), SlotIndex);
        return;
    }

    // 현재 장착 중인 무기 해제
    if (CurrentGun)
    {
        CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    // 새로운 무기 장착
    CurrentGun = NewWeapon;
    FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
    CurrentGun->AttachToComponent(GetMesh(), AttachRules, CurrentGun->GunSocketName);

    UE_LOG(LogTemp, Log, TEXT("Equipped weapon from slot %d: %s"), SlotIndex + 1, *CurrentGun->GetName());
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

            if (PlayerController->ESCAction)
            {
                // 메인메뉴 (ESC)
                EnhancedInput->BindAction(
                    PlayerController->ESCAction, 
                    ETriggerEvent::Triggered, 
                    PlayerController,
                    &AGJPlayerController::OpenMainMenu);
            }

            if (PlayerController->DropWeaponAction)
            {
                // 무기 드롭(G)
                EnhancedInput->BindAction(
                    PlayerController->DropWeaponAction, 
                    ETriggerEvent::Triggered, 
                    this, 
                    &AGJCharacter::DropWeapon);
            }

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
                    &AGJCharacter::EquipWeaponFromSlot, 0
                );
            }

            if (PlayerController->WeaponSlot2Action)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponSlot2Action,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::EquipWeaponFromSlot, 1
                );
            }

            if (PlayerController->WeaponSlot3Action)
            {
                EnhancedInput->BindAction(
                    PlayerController->WeaponSlot3Action,
                    ETriggerEvent::Triggered,
                    this,
                    &AGJCharacter::EquipWeaponFromSlot, 2
                );
            }
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
    if (!bCanJump) // 점프 가능 여부 체크
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
    if (GetCharacterMovement()->NavAgentProps.bCanCrouch) // 예외 처리
    {
        UnCrouch(); // 앉기 해제
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // 원래 속도로 복귀
    }
}

void AGJCharacter::OnDeath()
{
    // 추가적인 사망 후 로직 (예: 게임 오버 처리)
    if (GetController())
    {
        GetController()->DisableInput(nullptr);  // 입력 비활성화
        GetController()->UnPossess();  // 컨트롤러 해제 (더 이상 캐릭터 조작 불가)
    }

    UE_LOG(LogTemp, Error, TEXT("Player has died! Game Over"));

    // 레그돌 활성화
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));  // 충돌 프로파일을 래그돌로 변경
    GetMesh()->SetSimulatePhysics(true);  // 물리 시뮬레이션 활성화

    // 캡슐 콜라이더(캐릭터 충돌 상자) 비활성화
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

    // 일정 시간이 지나면 캐릭터 제거 (예: 10초 후)
    SetLifeSpan(10.0f);

    // 게임 오버 처리
    /*if (AGJGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AGJGameState>() : nullptr)
    {
        GameState->OnGameOver();
    }*/
}
