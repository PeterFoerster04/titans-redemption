// Fill out your copyright notice in the Description page of Project Settings.


#include "PoliceStation.h"
#include "Camera/CameraComponent.h"
#include "Aircraft.h"
#include "Criminal.h"
#include "CriminalSpawnPointManager.h"
#include "PoliceStationUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APoliceStation::APoliceStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);

	m_TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_TriggerBox->SetupAttachment(m_Mesh);

	m_GarageCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	m_GarageCam->SetupAttachment(RootComponent);

	m_FileSelectionCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam2"));
	m_FileSelectionCam->SetupAttachment(RootComponent);

	m_PlayerGaragePos = CreateDefaultSubobject<USceneComponent>(TEXT("GaragePos"));
	m_PlayerGaragePos->SetupAttachment(RootComponent);

	m_PlayerOutSpawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerOutPos"));
	m_PlayerOutSpawnPos->SetupAttachment(RootComponent);

	m_File1SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("File1Pos"));
	m_File1SpawnPoint->SetupAttachment(RootComponent);

	m_File2SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("File2Pos"));
	m_File2SpawnPoint->SetupAttachment(RootComponent);

	m_File3SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("File3Pos"));
	m_File3SpawnPoint->SetupAttachment(RootComponent);



	m_PoliceUIManager = CreateDefaultSubobject<UPoliceStationUIManager>(TEXT("UIManager"));


	m_World = GetWorld();


	
}

void APoliceStation::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AAircraft* player = Cast<AAircraft>(OtherActor);
	if(player != nullptr &&!m_PLayerIsInGarage)
	{
		//UE_LOG(LogTemp, Display, TEXT("Trigger Pawn"))
		//initiating the park in sequence
		m_PLayerIsInGarage = true;
		m_PlayerVehicle->m_InputBlocked = true;
		m_ParkingPlayerIn = true;
		m_StartFade = true;
		if(m_PoliceUIManager->m_PlayerHUDWidget!=nullptr)m_PoliceUIManager->m_PlayerHUDWidget->RemoveFromParent();
		
	}
}

// Called when the game starts or when spawned
void APoliceStation::BeginPlay()
{
	Super::BeginPlay();

	m_GameInstance = Cast<USpeederChaseGameInstance>(GetGameInstance());

	if(m_World !=nullptr) m_PlayerController = UGameplayStatics::GetPlayerController(m_World, 0);
	if(m_PlayerController != nullptr) m_PlayerVehicle = Cast<AAircraft>(m_PlayerController->GetPawn());
	if (m_PlayerVehicle != nullptr) m_PlayerStats = m_PlayerVehicle->GetComponentByClass<UPlayerStats>();
	m_TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APoliceStation::OnBoxBeginOverlap);

	if(m_PlayerWantsNewMissions) CreateNewFiles();
	/*if(m_PlayerHasChosenFile)
	{
		CreateMissionBasedOnFile();
	}*/
	m_CurrTime = m_TimerDuration;
	m_CriminalSpawnPos = FVector(GetActorLocation().X,GetActorLocation().Y-10000,GetActorLocation().Z+10000);
}

void APoliceStation::CreateNewFiles()
{
	CreateAndConfigureFile(&m_CaseFile1, m_File1SpawnPoint);
	CreateAndConfigureFile(&m_CaseFile2, m_File2SpawnPoint);
	CreateAndConfigureFile(&m_CaseFile3, m_File3SpawnPoint);

	m_PlayerWantsNewMissions = false;
}

void APoliceStation::DeleteOldFiles()
{
	if(m_CaseFile1 != nullptr)
	{
		m_CaseFile1->Destroy();
		m_CaseFile1 = nullptr;
	}
	if (m_CaseFile2 != nullptr)
	{
		m_CaseFile2->Destroy();
		m_CaseFile2 = nullptr;
	}
	if (m_CaseFile3 != nullptr)
	{
		m_CaseFile3->Destroy();
		m_CaseFile3 = nullptr;
	}
	

}

void APoliceStation::CreateAndConfigureFile(ACaseFile** a_File, USceneComponent* a_SpawnPos)
{
	//calculating all values before parsing them into the file creator
	float multi = CalculateSpeedMultiplier();
	float reward = CalculateMissionReward();
	float speed = PredictMaxSpeedInKmph();
	int spawnIndex = PickRandomSpawnPos();

	if ((*a_File) != nullptr) (*a_File)->Destroy();
	(*a_File) = m_World->SpawnActor<ACaseFile>(m_BPFile, a_SpawnPos->GetComponentLocation(), a_SpawnPos->GetRelativeRotation()+GetActorRotation());
	(*a_File)->SetCustomFileValues(reward, speed,multi,spawnIndex);
	//call blueprintEvent
	(*a_File)->CustomValuesToWidget();

}

int APoliceStation::PickRandomSpawnPos()
{
	int spawnIndex = FMath::RandRange(0, 6);
	return spawnIndex;
}

FVector APoliceStation::ConvertFromIndexToPosition(int a_Index)
{
	//this function takes an index and picks from the spawn manager that just has a list of spawnpoints
	if(m_CriminalSpawnManager != nullptr)
	{
		return m_CriminalSpawnManager->m_SpawnPositions[a_Index];
	}
	return FVector();
}

void APoliceStation::CreateMissionBasedOnFile()
{
	//Uobjects can not have parameters in constructors
	//so initializing all variables when object is constructed
	m_CurrentMission = m_World->SpawnActor<ACaseMission>(ACaseMission::StaticClass());
	m_CurrentMission->InitMission(m_SelectedFile, FMath::FRandRange(m_MinMissionTimeLimit, m_MaxMissionTimeLimit),m_CurrentCriminal);
}

float APoliceStation::CalculateMissionReward()
{
	//reward is calculated starting from min multiplier
	float reward = m_RandomSpeedMultiplier - m_MinSpeedMultiplier+m_MinReward;
	//apply multiplier if criminal is faster than you
	if (m_RandomSpeedMultiplier > 6000.0f) reward *= 1.5f;
	return reward;
}

float APoliceStation::CalculateSpeedMultiplier()
{
	return m_RandomSpeedMultiplier = FMath::RandRange(m_MinSpeedMultiplier, m_MaxSpeedMultiplier);
}

float APoliceStation::PredictMaxSpeedInKmph()
{
	return m_RandomSpeedMultiplier * 0.036f;
}

void APoliceStation::CalculateBonusReward()
{
	//no bonus if last minute was hit
	if (m_CurrentMission->m_CurrentTime < 60.0f) return;
	m_BaseRewardForAbsolvedMission = m_CurrentMission->m_MissionReward;
	//for every minute left in the mission, the player is rewarded with a 10% boost on the reward
	//converting left time into minutes and dividing by 10 to get a 10% per minute
	m_TimeBonusReward = m_CurrentMission->m_MissionReward*((m_CurrentMission->m_CurrentTime / 60.0f) / 10.0f);
	

}

void APoliceStation::DeInitMission()
{
	m_PlayerStats->m_IsCurrentlyInMission = false;
	m_PlayerStats->m_HackedCriminalVehicle = false;
	m_CurrentCriminal->Destroy();
	m_CurrentCriminal = nullptr;
	m_CurrentMission->Destroy();
	m_CurrentMission = nullptr;
	m_PlayerWantsNewMissions = true;
}

void APoliceStation::SpawnCriminal()
{
	if(m_Criminal1BP != nullptr)
	{
		m_CurrentCriminal = m_World->SpawnActor<ACriminal>(m_Criminal1BP, ConvertFromIndexToPosition(m_SelectedFile->m_SpawnIndex), FRotator::ZeroRotator);
	}
}

void APoliceStation::ConfigureCriminalValues()
{
	if(m_CurrentCriminal != nullptr)
	{
		m_CurrentCriminal->m_CriminalStats->m_SpeedMultiplier = m_SelectedFile->m_CriminalSpeedMultiplier;
	}
}

// Called every frame
void APoliceStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//stopping mission if timer is 0 or criminal has no hp anymore
	if(m_CurrentMission != nullptr && m_CurrentMission->m_WasActivated)
	{
		if(m_PlayerStats->m_HackedCriminalVehicle)
		{
			//first calculating than granting and saving and then deleting
			//fail and win screen functions are called in blueprint
			CalculateBonusReward();
			m_CurrentMission->m_MissionWasAbsolved = true;
			m_MissionAbsolvedTime = m_CurrentMission->m_CurrentTime;
			m_MissionInitialTime = m_CurrentMission->m_TimeLimit;
			ShowWinScreen();
			m_PlayerStats->m_CurrentMoney += m_CurrentMission->m_MissionReward;
			m_PlayerStats->m_CurrentMoney += m_TimeBonusReward;
			m_GameInstance->m_Saves->m_PlayerMoney = m_PlayerStats->m_CurrentMoney;
			DeInitMission();
			return;
		}
		if(m_CurrentCriminal->m_CriminalStats->m_VehicleCurrentHP <=0||m_CurrentMission->m_CurrentTime <=0)
		{
			m_CurrentMission->m_MissionFailed = true;
			if(m_CurrentMission->m_CurrentTime <= 0)ShowFailScreen(true);
			else ShowFailScreen(false);
			
			DeInitMission();
			return;
		
		}
	}
	if (!m_PLayerIsInGarage) return;

	//just played around with some self-made timers and not using unreals stuff lol
	if (m_StartFade)
	{
		UGameplayStatics::GetPlayerCameraManager(m_World, 0)->StartCameraFade(0.0f, 1.0f, m_TimerDuration - 0.5f, FLinearColor::Black, false, true);
		m_StartFade = false;
		m_TimerCounts = true;
	}
	if (m_TimerCounts)
	{
		m_CurrTime -= DeltaTime;
		if (m_CurrTime < 0)
		{
			m_TimerCounts = false;
			m_CurrTime = m_TimerDuration;
			m_PoliceUIManager->SwapToGarageMenu();
			m_PlayerController->SetViewTargetWithBlend(this);
			UGameplayStatics::GetPlayerCameraManager(m_World, 0)->StartCameraFade(1.0f, 0.0f, 0.5f, FLinearColor::Black);

		}
	}

	if (m_ParkingPlayerIn)
	{
		//returns true if still parking in
		//parking player in until the correct spot is reached
		if (!m_PlayerVehicle->ParkVehicleInGarage(m_PlayerGaragePos->GetComponentLocation(), m_PlayerGaragePos->GetComponentRotation(), DeltaTime))
		{
			m_PlayerVehicle->m_LeftMissileLauncher->m_CurrentRocketAmount = m_PlayerVehicle->m_LeftMissileLauncher->m_MaxRocketAmount;
			m_PlayerVehicle->m_RightMissileLauncher->m_CurrentRocketAmount = m_PlayerVehicle->m_RightMissileLauncher->m_MaxRocketAmount;
			m_ParkingPlayerIn = false;
		}
	}
}

void APoliceStation::KickPlayerOutOfGarage()
{
	//resetting some tool values when exiting the garage, reload etc.
	m_PlayerController->SetViewTargetWithBlend(m_PlayerVehicle);
	m_PoliceUIManager->SwapToPlayerHUD();
	m_PlayerVehicle->m_InputBlocked = false;
	m_PLayerIsInGarage = false;
	m_ParkingPlayerIn = false;
	m_PlayerVehicle->m_LeftMissileLauncher->m_CurrentRocketAmount = m_PlayerVehicle->m_LeftMissileLauncher->m_MaxRocketAmount;
	m_PlayerVehicle->m_RightMissileLauncher->m_CurrentRocketAmount = m_PlayerVehicle->m_RightMissileLauncher->m_MaxRocketAmount;
	m_PlayerVehicle->m_EMPCannon->m_WasFired = false;
	m_PlayerVehicle->m_EMPCannon->m_WasDetonated = false;
	m_PlayerVehicle->m_EMPCannon->m_ChargePercentage = 0.0f;
	m_PlayerVehicle->m_EMPCannon->m_CurrentEMPChargeTime = m_PlayerVehicle->m_EMPCannon->m_EMPChargeTime;
	
	m_PlayerVehicle->EjectVehicleOutside(m_PlayerOutSpawnPos->GetComponentLocation(), m_PlayerOutSpawnPos->GetComponentRotation());
}

void APoliceStation::InitiateSelectedMission()
{
	// do nothing if mission already exists
	if (m_CurrentMission != nullptr||m_SelectedFile == nullptr) return;
	//just a test, setting selected file to first one
	
	SpawnCriminal();
	ConfigureCriminalValues();
	CreateMissionBasedOnFile();
	m_CurrentMission->m_WasActivated = true;
	m_PlayerStats->m_CurrentMission = m_CurrentMission;
	m_PlayerStats->m_IsCurrentlyInMission = true;
	KickPlayerOutOfGarage();
	SwapPoliceCams();
}

void APoliceStation::SwapPoliceCams()
{
	m_FileSelectionCam->ToggleActive();
	m_GarageCam->ToggleActive();
};

