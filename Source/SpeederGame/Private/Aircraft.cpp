// Fill out your copyright notice in the Description page of Project Settings.


#include "Aircraft.h"
#include "EMPProjectile.h"
#include "Criminal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MathUtil.h"
#include "Components/PostProcessComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "SpeederChaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAircraft::AAircraft()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	 
	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);

	m_PostP = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	m_PostP->SetupAttachment(RootComponent);

	m_MiniMapSpringArm= CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringarm"));
	m_MiniMapSpringArm->SetupAttachment(m_Mesh);

	m_BoosterRNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BoosterRNS"));
	m_BoosterRNS->SetupAttachment(m_Mesh);

	m_BoosterLNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BoosterLNS"));
	m_BoosterLNS->SetupAttachment(m_Mesh);

	m_ThrusterRNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ThrusterRNS"));
	m_ThrusterRNS->SetupAttachment(m_Mesh);

	m_ThrusterLNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ThrusterLNS"));
	m_ThrusterLNS->SetupAttachment(m_Mesh);

	m_SpeedLinesNS = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpeedLinesNS"));
	m_SpeedLinesNS->SetupAttachment(m_Mesh);
	
	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Springarm"));
	m_SpringArm->SetupAttachment(m_Mesh);

	m_GunLeftFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("LeftGunPos"));
	m_GunLeftFirePos->SetupAttachment(m_Mesh);

	m_GunRightFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("RightGunPos"));
	m_GunRightFirePos->SetupAttachment(m_Mesh);

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_SpringArm);

	m_Hook = CreateDefaultSubobject<UTitanHook>(TEXT("Hook"));
	m_Hook->SetupAttachment(m_Mesh);

	m_HookCable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	m_HookCable->SetupAttachment(m_Mesh);
	
	m_HookInitPos = CreateDefaultSubobject<USceneComponent>(TEXT("HookInitPos"));
	m_HookInitPos->SetupAttachment(m_Mesh);

	m_StunTurret = CreateDefaultSubobject<UTitanStunTurret>(TEXT("Turret"));
	m_StunTurret->SetupAttachment(m_Mesh);

	m_TurretLeftFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("LeftTurretPos"));
	m_TurretLeftFirePos->SetupAttachment(m_StunTurret);

	m_TurretRightFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("RightTurretPos"));
	m_TurretRightFirePos->SetupAttachment(m_StunTurret);

	m_LeftMissileLauncher = CreateDefaultSubobject<UTitanMissileLauncher>(TEXT("LeftML"));
	m_LeftMissileLauncher->SetupAttachment(m_Mesh);

	m_RightMissileLauncher = CreateDefaultSubobject<UTitanMissileLauncher>(TEXT("RightML"));
	m_RightMissileLauncher->SetupAttachment(m_Mesh);

	m_MissileLeftFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("MissileFirePosL"));
	m_MissileLeftFirePos->SetupAttachment(m_LeftMissileLauncher);

	m_MissileRightFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("MissileFirePosR"));
	m_MissileRightFirePos->SetupAttachment(m_RightMissileLauncher);

	m_EMPCannon = CreateDefaultSubobject<UTitanEMPCannon>(TEXT("EMPCannon"));
	m_EMPCannon->SetupAttachment(m_Mesh);

	m_EMPFirePos = CreateDefaultSubobject<USceneComponent>(TEXT("EMPFirePos"));
	m_EMPFirePos->SetupAttachment(m_EMPCannon);

	m_Stats = CreateDefaultSubobject<UPlayerStats>(TEXT("Stats"));
}

// Called when the game starts or when spawned
void AAircraft::BeginPlay()
{
	Super::BeginPlay();

	m_World = GetWorld();
	const APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* inputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	if (inputSystem != nullptr && mappingContext != nullptr) {
		inputSystem->AddMappingContext(mappingContext, 0);
	}
	m_SpringArmInitPos = m_SpringArm->GetRelativeLocation();
	m_PostP->Settings.bOverride_SceneFringeIntensity = true;
	m_PostP->Settings.bOverride_VignetteIntensity = true;
	m_Mesh->OnComponentHit.AddDynamic(this, &AAircraft::OnHit);
	m_Hook->OnComponentHit.AddDynamic(this, &AAircraft::OnHookCompHit);
	m_StunProjectile.GetDefaultObject()->m_IsStun = true;

	//setting all default values
	m_BlasterShotsLeft = m_MaxBlasterShots;
	m_CurrentBlasterRechargeTime = m_BlasterRechargeTime;

	m_StunTurret->m_StunShotsLeft = m_StunTurret->m_MaxStunShots;
	m_StunTurret->m_CurrentStunRechargeTime = m_StunTurret->m_StunRechargeTime;

	m_VehicleCurrentShields = m_VehicleMaxShields;
	m_VehicleCurrentHP = m_VehicleMaxHP;
	m_CurrentHookHack = m_HookHackTime;

	m_LeftMissileLauncher->m_CurrentRocketAmount = m_LeftMissileLauncher->m_MaxRocketAmount;
	m_RightMissileLauncher->m_CurrentRocketAmount = m_RightMissileLauncher->m_MaxRocketAmount;

	m_EMPCannon->m_CurrentEMPChargeTime = m_EMPCannon->m_EMPChargeTime;

	//loading all saved values
	USpeederChaseGameInstance* game = Cast<USpeederChaseGameInstance>(GetGameInstance());
	m_MouseSens = game->m_Saves->m_MouseSens;
	
	m_Stats->m_CurrentMoney = game->m_Saves->m_PlayerMoney;
	m_RepairedBooster = game->m_Saves->m_RepairedBooster;
	m_LeftMissileLauncher->m_AttachmentRepaired = game->m_Saves->m_RepairedMissileLauncher;
	m_RightMissileLauncher->m_AttachmentRepaired = game->m_Saves->m_RepairedMissileLauncher;
	m_StunTurret->m_AttachmentRepaired = game->m_Saves->m_RepairedStun;
	m_EMPCannon->m_AttachmentRepaired = game->m_Saves->m_RepairedEMP;
}

void AAircraft::OnHookCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!m_HookIsExtended) return;
	//differentiating between hitting the criminal or a static building
	ACriminal* criminal = Cast<ACriminal>(OtherActor);
	
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr&&criminal!=nullptr)
	{
		//if the hook hits something that moves it has to be attached to the object
		UE_LOG(LogTemp, Display, TEXT("Hook Event Vehicle"));
		m_Hook->m_HookedSomething = true;
		m_HookedCriminal = true;
		m_Hook->SetSimulatePhysics(false);
		m_Hook->SetPhysicsLinearVelocity(FVector::Zero());
		m_Hook->AttachToComponent(criminal->m_StaticMesh, FAttachmentTransformRules::KeepWorldTransform);
		//make the criminal not hook-able with shields
		if (criminal->m_CriminalStats->m_VehicleCurrentShields > 0)
		{
			m_RetractHook = true;
		}
	}
	else if(OtherActor != nullptr&&OtherActor != this &&OtherComp != nullptr)
	{
		//just setting the new position for static objects
		UE_LOG(LogTemp, Display, TEXT("Hook Event no Vehicle"));
		m_Hook->m_HookedSomething = true;
		m_HookImpactPoint = Hit;
		m_Hook->SetPhysicsLinearVelocity(FVector::Zero());
		m_Hook->SetSimulatePhysics(false);
	}
	
	if (!m_PlayedHookAttachSound&&m_HookIsExtended)
	{
		UGameplayStatics::PlaySound2D(m_World, m_HookAttachSound);
		m_PlayedHookAttachSound = true;
	}
}

void AAircraft::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ACriminal* criminal = Cast<ACriminal>(OtherActor);

	if (m_IsOnCrashDelay) return;
	if(criminal != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Criminal Player Vehicle Crash"))
		criminal->TakeCriticalDamage(20.0f);
		//delay gets reset in blueprint of this class
		m_IsOnCrashDelay = true;
	}

}

void AAircraft::PortPlayerBackToSpawn(FVector a_SpawnPos)
{
	m_Mesh->SetWorldLocation(a_SpawnPos);
}

void AAircraft::ChangeBoosterMaterialGlow(float a_RedValue, float a_YellowValue)
{
	m_Mesh->SetScalarParameterValueOnMaterials("RedGlowValue",a_RedValue);
	m_Mesh->SetScalarParameterValueOnMaterials("YellowGlowValue", a_YellowValue);
}

void AAircraft::AddMovementForce(float a_DeltaTime)
{
	//multiplying thrust and boost values to current velocity to move pawn
	//rotations are applied in callback functions
	const FVector currentVelocity = m_Mesh->GetPhysicsLinearVelocity();
	//if player boosts, multiplier is changed to the actual value
	//thrust speed is modified depending on the current gear
	m_CurrentBoostMultiply = m_Boosting ? m_BoostMultiplier : 1.0f;
	const FVector forceToAdd = m_Thrust * (m_ThrustSpeed*(m_CurrentGear/m_MaxGear)) * m_CurrentBoostMultiply * m_Mesh->GetForwardVector();
	//lerping to mimic acceleration 
	const FVector newVelocity = FMath::Lerp(currentVelocity, forceToAdd, 0.2f*a_DeltaTime*50.0f);
	m_Mesh->SetPhysicsLinearVelocity(newVelocity);
	
}

void AAircraft::ShakeCamera(float a_DeltaTime)
{
	//simple custom cam shake, using unreal cam shake later on
	m_SpringArm->SetRelativeLocation(m_SpringArmInitPos + FMath::VRand()*m_ShakeAmount* a_DeltaTime *m_BoostMultiplier);
}

void AAircraft::CalculateKmpH(float a_DeltaTime)
{
	//converting to km/h
	
	float currVelocity = (m_Mesh->GetPhysicsLinearVelocity().Length()) * 0.036f;
	//smooth out the changing text
	m_Speedkmph = FMathf::Lerp(m_Speedkmph,currVelocity,0.08f);
}

void AAircraft::RotateMinimap()
{
	/*m_MiniMapCam->SetRelativeRotation(FRotator(0,0, m_Mesh->GetRelativeRotation().Yaw));*/
}

void AAircraft::HandleHook(float a_DeltaTime)
{
	//the following situations are managed in here:
	//shooting the hook to a certain max distance
	//retracting the hook if the cable exceeds a certain length
	//retracting the hook if player wants to

	if(m_HookIsExtended)
	{
		FVector distance = m_Hook->GetComponentLocation().GetAbs() - m_Mesh->GetComponentLocation().GetAbs();
		UE_LOG(LogTemp, Display, TEXT("Hook Dist:%f"), distance.Length());
		if(!m_Hook->m_HookedSomething&&distance.Length()<7000.0f)
		{
			//shooting the hook to the limited distance
			m_Hook->SetSimulatePhysics(true);
			if(m_HookAimLockActive)
			{
				UE_LOG(LogTemp, Display, TEXT("Aim Assist Travel"));
				m_Hook->SetPhysicsLinearVelocity(m_Mesh->GetPhysicsLinearVelocity() + m_HookAimLockDirection.Vector() * 5000.0f * a_DeltaTime * 80.0f);
				UE_LOG(LogTemp, Display, TEXT("Assist Direct: %f,%f,%f"), m_HookAimLockDirection.Vector().X, m_HookAimLockDirection.Vector().Y, m_HookAimLockDirection.Vector().Z);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("No Aim Assist Travel"));
				m_Hook->SetPhysicsLinearVelocity(m_Mesh->GetPhysicsLinearVelocity() + m_Mesh->GetForwardVector() * 5000.0f * a_DeltaTime * 80.0f);
			}
		}
		else
		{
			//limiting the hook cable length 
			if (distance.Length() > 10000.0f) m_RetractHook = true;

			if(m_Hook->m_HookedSomething&&!m_RetractHook&&!m_HookedCriminal)
			{
				//if hook hits something that does not move just setting location
				m_Hook->SetPhysicsLinearVelocity(FVector::Zero());
				m_Hook->SetWorldLocation(m_HookImpactPoint.ImpactPoint);
				UE_LOG(LogTemp, Display, TEXT("Hooked"));
			}
			else if(m_Hook->m_HookedSomething && !m_RetractHook&&m_HookedCriminal)
			{
				UE_LOG(LogTemp, Display, TEXT("Hooked Criminal"));
			}
			else 
			{
				RetractHook();
			}
		}
	}
}

void AAircraft::RetractHook()
{
	//resetting all values and reattaching it to player
	//to reattach the hook to player, the player vehicles physics have
	//to be disabled for a moment, there is no other way
	//this sadly causes a little stutter when reattaching and moving at the same time
	//but this is actually okay, simulates a little recoil
	UE_LOG(LogTemp, Display, TEXT("Retract Hook"));
	UGameplayStatics::PlaySound2D(m_World, m_HookRetractSound);
	m_HookedCriminal = false;
	m_Mesh->SetSimulatePhysics(false);
	m_Hook->SetSimulatePhysics(false);
	m_Hook->AttachToComponent(m_Mesh, FAttachmentTransformRules::KeepWorldTransform);
	m_Hook->SetWorldRotation(m_Mesh->GetComponentRotation());
	m_Hook->SetWorldLocation(m_HookInitPos->GetComponentLocation());
	m_Mesh->SetSimulatePhysics(true);
	m_HookIsExtended = false;
	m_RetractHook = false;
	m_Hook->m_HookedSomething = false;
	m_PlayedHookAttachSound = false;
	m_HookAimLockActive = false;
}

void AAircraft::UpdateFlightVisualFeedback(float a_DeltaTime)
{

	//changing fov, spring arm length, booster material and shaking camera, depends, if vehicle is thrusting or boosting
	//also handles sounds, function just handles all the feedback for flying :)
	//the value in the alpha slot for some lerp functions was introduced to counter against delta time to get same smoothing as before,
	//so do not wonder why there are magic numbers, they are literally magical
	if (m_Thrust > 0)
	{
		//increasing fov when thrusting, takes boosting into account
		m_Camera->FieldOfView = FMathf::Lerp(m_Camera->FieldOfView, m_DefaultFOV + (25.0f * m_CurrentBoostMultiply*(m_CurrentGear/m_MaxGear)), 2.0f * a_DeltaTime);
	}
	else
	{
		m_Camera->FieldOfView = FMathf::Lerp(m_Camera->FieldOfView, m_DefaultFOV, 2.0f * a_DeltaTime);
	}
	if (m_Boosting && m_Thrust > 0 && m_CurrentGear == 5 && m_BoostCharge > 0.0f)
	{
		m_SpringArm->TargetArmLength = FMathf::Lerp(m_SpringArm->TargetArmLength, 700.0f, 2.0f * a_DeltaTime);
		ChangeBoosterMaterialGlow(5.0f, 10.0f);
		ShakeCamera(a_DeltaTime);
		m_PostP->Settings.VignetteIntensity = FMathf::Lerp(m_PostP->Settings.VignetteIntensity, 1.2f, 2.0f * a_DeltaTime);
		ConsumeBoostCharge(a_DeltaTime);
		if(!m_SpawnedBoosterAudio)
		{
			//preventing the sound to spawn multiple times
			UGameplayStatics::PlaySoundAtLocation(m_World, m_BoosterPowerUpSound,m_Mesh->GetComponentLocation());
			m_BoosterAudioComp = UGameplayStatics::SpawnSound2D(m_World, m_BoostQuantumSound);
			m_SpawnedBoosterAudio = true;
		}
	}
	else
	{
		m_SpringArm->TargetArmLength = FMathf::Lerp(m_SpringArm->TargetArmLength, 400.0f, 2.0f * a_DeltaTime);
		ChangeBoosterMaterialGlow(1.0f, 0.0f);
		m_PostP->Settings.VignetteIntensity = FMathf::Lerp(m_PostP->Settings.VignetteIntensity, 0.0f, 2.0f * a_DeltaTime);
		//if for example gear is shifted to a gear under 5, the booster has to be canceled one time, since the callback function is not called to do so
		if (m_BoosterIsCancelable)
		{
			DeactivateBoosterSystems();
			m_Boosting = false;
			m_BoosterIsCancelable = false;
		}
	}
}

void AAircraft::TimePlayerHookCriminal(float a_DeltaTime)
{
	//this function handles the hacking of the criminal vehicle , just being a simple timer that resets itself
	//the police station closes the mission if the criminal wqs successfully hacked
	if(m_HookedCriminal)
	{
		if(m_CurrentHookHack > 0.0f)
		{
			m_CurrentHookHack -= a_DeltaTime;
			UE_LOG(LogTemp, Display, TEXT("Hack:%f"),m_CurrentHookHack);
		}
		else
		{
			m_CurrentHookHack = 0.0f;
			m_Stats->m_HackedCriminalVehicle = true;
			m_CurrentHookHack = m_HookHackTime;
			m_RetractHook = true;
		}
	}
	else
	{
		m_CurrentHookHack = m_HookHackTime;
	}
}

void AAircraft::TickEMPTimer(float a_DeltaTime)
{
	if (!m_Stats->m_IsCurrentlyInMission) return;
	//since the emp can also detonate on contact it is crucial to keep track fo the current state
	if (m_EMPCannon->m_WasFired) return;

	m_EMPCannon->ChargeEMP(a_DeltaTime);
	
}

void AAircraft::UpdateBoostCharge(float a_DeltaTime)
{
	//cooldown prevents spamming boost when it goes out, gaining free extra speed infinitely
	//else the boost will just charge up normally 
	if(m_BoostIsOnCooldown &&m_BoostCooldown>0)
	{
		m_BoostCooldown -= a_DeltaTime;
		return;
	}
	if(m_BoostIsOnCooldown&& m_BoostCooldown < 0)
	{
		m_BoostIsOnCooldown = false;
		m_BoostCooldown = 5.0f;
		
	}
	if(m_BoostCharge <m_MaxBoostCharge &&!m_Boosting)
	{
		m_BoostCharge += m_BoostRechargeAmount*a_DeltaTime;
	}
	else if(m_BoostCharge > m_MaxBoostCharge && !m_Boosting)
	{
		m_BoostCharge = m_MaxBoostCharge;
	}
}

void AAircraft::ConsumeBoostCharge(float a_DeltaTime)
{
	//discharging the boost for every frame it is used, id it is completely used down to 0, the cooldown hits
	m_BoostCharge -= m_BoostConsumption * a_DeltaTime;
	if(m_BoostCharge <0.0f)
	{
		m_BoostCharge = 0.0f;
		m_BoostIsOnCooldown = true;
	}
}

void AAircraft::DeactivateBoosterSystems()
{
	m_BoosterRNS->Deactivate();
	m_BoosterLNS->Deactivate();
	m_SpeedLinesNS->Deactivate();
	m_PostP->Settings.SceneFringeIntensity = 0.0f;
	if(m_BoosterAudioComp != nullptr)m_BoosterAudioComp->FadeOut(1.5f, 0.0f);
	UGameplayStatics::PlaySoundAtLocation(m_World, m_BoosterPowerDownSound,m_Mesh->GetComponentLocation());
	m_SpawnedBoosterAudio = false;
	//m_BoosterAudioComp->Stop();
}

void AAircraft::RechargeBlasters(float a_DeltaTime)
{
	//only recharge if needed, shoot is then on cooldown, gets recharged after
	if (m_BlasterShotsLeft > 0) return;
	if(m_CurrentBlasterRechargeTime >0)m_CurrentBlasterRechargeTime -= a_DeltaTime;
	else{
		m_BlasterShotsLeft = m_MaxBlasterShots;
		m_CurrentBlasterRechargeTime = m_BlasterRechargeTime;
	}
}

void AAircraft::ManagePlayerLocation()
{
	//so that the player does not fly out of map :)
	if(m_Mesh->GetComponentLocation().Length()> 700000.0f&&!m_PlayerOutOfMapRange)
	{
		m_PlayerOutOfMapRange = true;
	}
}
//next are some input functions
//since no movement controller compoent was used, blocking input has to be done manually
//usually there is just a function to disable input

void AAircraft::RollInput(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;

	float roll = a_Value.Get<float>();

	m_Mesh->AddTorqueInRadians(m_Mesh->GetForwardVector() * m_RollMultiplier * roll, NAME_None, true);
}

void AAircraft::YawInput(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	float yaw = a_Value.Get<float>();
	yaw *= m_MouseSens;
	yaw = FMathf::Clamp(yaw, -m_MaxMouseInput, m_MaxMouseInput);
	m_Mesh->AddTorqueInRadians(m_Mesh->GetUpVector() * m_YawMultiplier * yaw, NAME_None, true);

	//UE_LOG(LogActor, Error, TEXT("Yaw:%f"), yaw);
}

void AAircraft::PitchInput(const FInputActionValue& a_Value) 
{
	if (m_InputBlocked) return;
	float pitch = a_Value.Get<float>();
	pitch *= m_MouseSens;
	pitch = FMathf::Clamp(pitch, -m_MaxMouseInput, m_MaxMouseInput);
	m_Mesh->AddTorqueInRadians(m_Mesh->GetRightVector() * m_PitchMultiplier * pitch, NAME_None, true);
}

void AAircraft::ThrustInput(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) {
		m_Thrust = 0;
		m_ThrusterLNS->SetVariableFloat(FName("ParticleVelocity"), 0.0f);
		m_ThrusterRNS->SetVariableFloat(FName("ParticleVelocity"), 0.0f);
		return;
	}
	m_Thrust = a_Value.Get<float>();
	if(m_Thrust >0)
	{
		m_ThrusterLNS->SetVariableFloat(FName("ParticleVelocity"), 20.0f * m_CurrentBoostMultiply);
		m_ThrusterRNS->SetVariableFloat(FName("ParticleVelocity"), 20.0f * m_CurrentBoostMultiply);
	}
	else
	{
		m_ThrusterLNS->SetVariableFloat(FName("ParticleVelocity"), 0.0f);
		m_ThrusterRNS->SetVariableFloat(FName("ParticleVelocity"), 0.0f);
	}


}

void AAircraft::BoostInput(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	if (!m_RepairedBooster || !m_BoosterActive) return;
	m_Boosting = a_Value.Get<bool>();
	if(m_Boosting&&m_Thrust >0)
	{
		m_BoosterRNS->Activate();
		m_BoosterLNS->Activate();
		m_SpeedLinesNS->Activate();
		m_PostP->Settings.SceneFringeIntensity = 2.0f;
		m_BoosterIsCancelable = true;
	}
	else 
	{
		m_Boosting = false;
		m_BoosterIsCancelable = false;
		DeactivateBoosterSystems();
	}
	

}

void AAircraft::ToggleLightsInput(const FInputActionValue& a_Value)
{
	//the vehicle actually has no lights lol
	//there is no night scene so it is disabled
	if (m_InputBlocked) return;
	bool lights = a_Value.Get<bool>();
}

void AAircraft::GunShoot(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	if (!m_GunsActive) return;
	if(!m_GunProjectile) return;
	if(m_BlasterShotsLeft ==0) return;

	ACriminal* currentCriminal = Cast<ACriminal>(UGameplayStatics::GetActorOfClass(m_World, ACriminal::StaticClass()));

	if(m_AimAssistActive && currentCriminal != nullptr)
	{
		GetWorld()->SpawnActor<AGunProjectile>(m_GunProjectile, m_GunLeftFirePos->GetComponentLocation(), UKismetMathLibrary::FindLookAtRotation(m_Mesh->GetComponentLocation(),currentCriminal->m_StaticMesh->GetRelativeLocation()));
		GetWorld()->SpawnActor<AGunProjectile>(m_GunProjectile, m_GunRightFirePos->GetComponentLocation(), UKismetMathLibrary::FindLookAtRotation(m_Mesh->GetComponentLocation(), currentCriminal->m_StaticMesh->GetRelativeLocation()));
	}
	else
	{
		GetWorld()->SpawnActor<AGunProjectile>(m_GunProjectile, m_GunLeftFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
		GetWorld()->SpawnActor<AGunProjectile>(m_GunProjectile, m_GunRightFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
	}
	

	m_BlasterShotsLeft--;
}

void AAircraft::TurretShoot(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	if (!m_StunTurret->m_AttachmentRepaired || !m_StunTurret->m_AttachmentActive) return;
	if (!m_StunProjectile) return;
	if(m_StunTurret->m_StunShotsLeft ==0) return;
	GetWorld()->SpawnActor<AGunProjectile>(m_StunProjectile, m_TurretLeftFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
	GetWorld()->SpawnActor<AGunProjectile>(m_StunProjectile, m_TurretRightFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
	m_StunTurret->m_StunShotsLeft--;
}

void AAircraft::ShootHook(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	//m_ActivatedHook = a_Value.Get<bool>();
	UE_LOG(LogTemp, Display, TEXT("Hook:Press"));
	if(!m_HookIsExtended&&!m_RetractHook)
	{
		UE_LOG(LogTemp, Display, TEXT("ExtendHook"));
		m_HookIsExtended = true;
		UGameplayStatics::PlaySound2D(m_World, m_HookShootSound);
		if (!m_AimAssistActive) return;
		UE_LOG(LogTemp, Display, TEXT("Aim Assist"));
		ACriminal* currentCriminal = Cast<ACriminal>(UGameplayStatics::GetActorOfClass(m_World, ACriminal::StaticClass()));
		if (currentCriminal == nullptr) return;
		UE_LOG(LogTemp, Display, TEXT("Found Criminal"));
		m_HookAimLockDirection = UKismetMathLibrary::FindLookAtRotation(m_Mesh->GetComponentLocation(), currentCriminal->m_StaticMesh->GetComponentLocation());
		m_HookAimLockActive = true;
		
	}
	else
	{
		m_HookAimLockActive = false;
		m_RetractHook = true;
	}
	
}

void AAircraft::LaunchMissiles(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	//just checking the left one every time is enough
	if (!m_LeftMissileLauncher->m_AttachmentRepaired || !m_LeftMissileLauncher->m_AttachmentActive) return;
	if (!m_Missile)return;
	if (m_LeftMissileLauncher->m_CurrentRocketAmount == 0) return;
	m_LeftMissileLauncher->m_CurrentRocketAmount--;
	m_RightMissileLauncher->m_CurrentRocketAmount--;
	GetWorld()->SpawnActor<AGunProjectile>(m_Missile, m_MissileLeftFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
	GetWorld()->SpawnActor<AGunProjectile>(m_Missile, m_MissileRightFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
}

void AAircraft::GearShift(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	float value = a_Value.Get<float>();

	if (value > 0 && m_CurrentGear < 5)
	{
		m_CurrentGear++;
	}
	else if(value<0&& m_CurrentGear>1)
	{
		m_CurrentGear--;
	}
	
}

void AAircraft::ToggleCurrentCaseFileOnScreen(const FInputActionValue& a_Value)
{
	bool value = a_Value.Get<bool>();
	m_CaseFileWidgetOnScreen = value;
	
}

void AAircraft::ShootEMP(const FInputActionValue& a_Value)
{
	if (m_InputBlocked) return;
	if (!m_EMPCannon->m_AttachmentRepaired || !m_EMPCannon->m_AttachmentActive) return;
	if (!m_EMPProjectile) return;

	//the emp first spawns the bomb that travels with some forward velocity, when the player presses the key for the first time
	//the bomb can be detonated manually when the player hits the key another time,or when it hits something

	if(m_EMPCannon->m_ChargePercentage != 100.0f) return;
	//first player key hit
	if(!m_EMPCannon->m_WasFired)
	{
		m_EMPCannon->m_WasFired = true;
		m_FiredEMPBomb = GetWorld()->SpawnActor<AEMPProjectile>(m_EMPProjectile, m_EMPFirePos->GetComponentLocation(), m_Mesh->GetRelativeRotation());
		
		UE_LOG(LogTemp, Display, TEXT("EMP"));
		return;
	}
	//second player key hit, if bomb still exists
	if(m_EMPCannon->m_WasFired &&!m_EMPCannon->m_WasDetonated)
	{

		if (m_FiredEMPBomb != nullptr&& !m_FiredEMPBomb->m_AlreadyDetonated)
		{
			ExplosionCamShake();
			m_FiredEMPBomb->m_AlreadyDetonated = true;
			m_FiredEMPBomb->Detonate();
		}
		m_EMPCannon->m_WasDetonated = true;
		m_EMPCannon->m_ChargePercentage = 0.0f;
		m_EMPCannon->m_CurrentEMPChargeTime = m_EMPCannon->m_EMPChargeTime;
	}
	
}

// Called every frame
void AAircraft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateMinimap();
	AddMovementForce(DeltaTime);
	UpdateFlightVisualFeedback(DeltaTime);
	CalculateKmpH(DeltaTime);
	HandleHook(DeltaTime);
	UpdateBoostCharge(DeltaTime);
	RechargeBlasters(DeltaTime);
	m_StunTurret->RechargeStunTurret(DeltaTime);
	TimePlayerHookCriminal(DeltaTime);
	TickEMPTimer(DeltaTime);
	ManagePlayerLocation();

	//UE_LOG(LogTemp,Display,TEXT("EMP CHARGE:%f"),m_EMPCannon->m_ChargePercentage)
}

// Called to bind functionality to input
void AAircraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	if((m_EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))== nullptr)
	{
		return;
	}

	m_EnhancedInput->BindAction(m_RollAction, ETriggerEvent::Triggered, this, &AAircraft::RollInput);
	m_EnhancedInput->BindAction(m_YawAction, ETriggerEvent::Triggered, this, &AAircraft::YawInput);
	m_EnhancedInput->BindAction(m_PitchAction, ETriggerEvent::Triggered, this, &AAircraft::PitchInput);
	m_EnhancedInput->BindAction(m_ThrustAction, ETriggerEvent::Triggered, this, &AAircraft::ThrustInput);
	m_EnhancedInput->BindAction(m_BoostAction, ETriggerEvent::Triggered, this, &AAircraft::BoostInput);
	m_EnhancedInput->BindAction(m_ToggleLightsAction, ETriggerEvent::Triggered, this, &AAircraft::ToggleLightsInput);
	m_EnhancedInput->BindAction(m_GunShootAction, ETriggerEvent::Triggered, this, &AAircraft::GunShoot);
	m_EnhancedInput->BindAction(m_TurretShootAction, ETriggerEvent::Triggered, this, &AAircraft::TurretShoot);
	m_EnhancedInput->BindAction(m_HookAction, ETriggerEvent::Triggered, this, &AAircraft::ShootHook);
	m_EnhancedInput->BindAction(m_MissileLaunchAction, ETriggerEvent::Triggered, this, &AAircraft::LaunchMissiles);
	m_EnhancedInput->BindAction(m_GearShiftAction, ETriggerEvent::Triggered, this, &AAircraft::GearShift);
	m_EnhancedInput->BindAction(m_ToggleCurrentCaseFile, ETriggerEvent::Triggered, this, &AAircraft::ToggleCurrentCaseFileOnScreen);
	m_EnhancedInput->BindAction(m_ShootEMPAction, ETriggerEvent::Triggered, this, &AAircraft::ShootEMP);
}

bool AAircraft::ParkVehicleInGarage(FVector a_Loc, FRotator a_Rot, float a_DeltaTime)
{
	//parking in until destination reached
	//returning true if still processing
	//this is called by the police station to park the vehicle into the garage
	m_Mesh->SetAllPhysicsLinearVelocity(FVector::Zero());
	m_Mesh->SetPhysicsAngularVelocityInRadians(FVector::Zero());

	if (m_Mesh->GetComponentLocation() != a_Loc ||m_Mesh->GetComponentRotation() != a_Rot) {
		m_Mesh->SetWorldLocation(FMath::Lerp(m_Mesh->GetComponentLocation(), a_Loc, 1.0f * a_DeltaTime));
		m_Mesh->SetWorldRotation(FMath::Lerp(m_Mesh->GetComponentRotation(), a_Rot, 1.0f * a_DeltaTime));
		return true;
	}
	return false;
	
}

void AAircraft::EjectVehicleOutside(FVector a_Loc, FRotator a_Rot)
{
	m_Mesh->SetWorldLocation(a_Loc);
	m_Mesh->SetWorldRotation(a_Rot);
}

