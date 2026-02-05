// Fill out your copyright notice in the Description page of Project Settings.


#include "CriminalController.h"
#include "Criminal.h"
#include "KismetTraceUtils.h"
#include "Kismet/KismetMathLibrary.h"


void ACriminalController::BeginPlay()
{
	//the behaviour tree was stupid xD
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	//m_Criminal = Cast<ACriminal>(GetPawn());
	m_Criminal = Cast<ACriminal>(GetInstigator());

	if(m_Criminal != nullptr)
	{
		//UE_LOG(LogTemp,Display,TEXT("Running Tree"))
		//RunBehaviorTree(m_Criminal->m_TreeAsset);
	}
	m_Pathfinder = new AStar(m_MeshGridAsset);
	UE_LOG(LogTemp, Display, TEXT("Blocked Paths Exist:"), m_MeshGridAsset->m_Grid[5907510]);
	
	
}

void ACriminalController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (m_Thread != nullptr)
	{
		//first letting the thread finish before ending, this prevents the game and engine to crash when closing
		m_Thread->Stop();
		//m_Thread->m_Thread->Kill();
		DeleteRawPointer(m_Thread);
	}
}

void ACriminalController::Tick(float DeltaSeconds)
{
	//if thread generated the path and is finished, it gets deleted
	Super::Tick(DeltaSeconds);
	if (!m_Move) return;


	if (m_Thread!=nullptr && m_Thread->m_FinishedTask&&m_UseMultithreading)
	{
		m_CalculatedPath = m_Thread->m_FinishedPath;
		//UE_LOG(LogTemp, Display, TEXT("Finshed Multithreaded Task."))
		m_CurrentVectorIndex = 0;
		m_NewPathRequired = false;
		m_StillCalculatingPath = false;
		DeleteRawPointer(m_Thread);
		//UE_LOG(LogTemp,Display,TEXT("First point to travel to: %f,%f,%f"),m_CalculatedPath[0].X, m_CalculatedPath[0].Y,m_CalculatedPath[0].Z)
	}



	//second bool is needed because task is multi threaded, checking if thread is running
	if(m_NewPathRequired&&!m_StillCalculatingPath)
	{
		CalculateNewPath();
	}
	else if(!m_NewPathRequired&&!m_StillCalculatingPath)
	{

		if(m_ArrivedAtNextPoint)
		{
			SetNextPositionOfPath();
		}
		else{
			FlyToNextPosition(DeltaSeconds);
		}
	}
	
	
}

ACriminalController::~ACriminalController()
{
	
	if(m_Pathfinder != nullptr)
	{
		DeleteRawPointer(m_Pathfinder);
	}

}

void ACriminalController::CalculateNewPath()
{

	if (m_MeshGridAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh Grid Asset Is Null LuL"))
			return;
	}
	//set this to 0 so the next finished path can be iterated
	//set fail condition to false so that the task returns success
	m_CurrentVectorIndex = 0;

	if(m_Criminal == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Criminal NUll"));
		return;
	}
	


	//first determining random destination and converting the current position into grid position
	FIntVector3 startpoint = ConvertFromWorldPosToGridPos(m_Criminal->m_StaticMesh->GetComponentLocation());
	FIntVector3 endpoint = CalculateRandomPosition(m_Criminal->m_StaticMesh->GetComponentLocation());

	//UE_LOG(LogTemp, Display, TEXT("Start:%d, %d, %d"), startpoint.X, startpoint.Y, startpoint.Z);
	//UE_LOG(LogTemp, Display, TEXT("Destination:%d, %d, %d"), endpoint.X, endpoint.Y, endpoint.Z);
	m_StillCalculatingPath = true;
	//creating new thread to perform calculation asynchronously
	if(m_UseMultithreading)
	{
		m_Thread = new PathFindingThread(m_Pathfinder, startpoint, endpoint);
	}
	else
	{
		m_CalculatedPath = m_Pathfinder->CalculatePath(startpoint, endpoint);
		m_CurrentVectorIndex = 0;
		m_NewPathRequired = false;
		m_StillCalculatingPath = false;
	}

	
}

void ACriminalController::SetNextPositionOfPath()
{
	//returns next vector fo list to travel to
	//if all vectors were iterated, bool gets flipped, for the behaviour tree task to fail


	if(m_CurrentVectorIndex < m_CalculatedPath.Num())
	{
		//UE_LOG(LogTemp, Display, TEXT("PathIndex: %d   ,%f,%f,%f"), m_CurrentVectorIndex, m_CalculatedPath[m_CurrentVectorIndex].X, m_CalculatedPath[m_CurrentVectorIndex].Y, m_CalculatedPath[m_CurrentVectorIndex].Z);
		FVector vec = m_CalculatedPath[m_CurrentVectorIndex];

		m_NextVectorElement = vec;
		m_CurrentVectorIndex++;
		m_ArrivedAtNextPoint = false;
		//UE_LOG(LogTemp, Display, TEXT("NextPointToTravelTo:%f,%f,%f"), vec.X, vec.Y, vec.Z);
	}
	else
	{
		m_NewPathRequired = true;
	}
	


}

void ACriminalController::FlyToNextPosition(float a_DeltaTime)
{
	//this function takes the next element of the path array and sets it as the next destination 

	FVector currLoc = m_Criminal->m_StaticMesh->GetComponentLocation();
	float dist = FVector::Distance(currLoc, m_NextVectorElement);
	//UE_LOG(LogTemp, Display, TEXT("Dist:%f"),dist);

	if (FVector::Distance(currLoc, m_NextVectorElement) > 200.0f)
	{
		FVector velocityToDestination = m_NextVectorElement - m_Criminal->m_StaticMesh->GetComponentLocation();
		FVector currentVel = m_Criminal->m_StaticMesh->GetPhysicsLinearVelocity();

		velocityToDestination.Normalize();

		FVector newVelocity = velocityToDestination * m_Criminal->m_CriminalStats->m_SpeedMultiplier;

		m_Criminal->m_StaticMesh->SetPhysicsLinearVelocity(FMath::Lerp(currentVel,newVelocity,0.2f * a_DeltaTime*50.0f));



		FRotator currRot = m_Criminal->m_StaticMesh->GetRelativeRotation();

		FRotator desiredRot = UKismetMathLibrary::FindLookAtRotation(m_Criminal->m_StaticMesh->GetComponentLocation(), m_NextVectorElement);
		//smoothing the flying a little bit
		//also controlling and handling the physics of the vehicle, because on crashing with the player this can cause issues
		m_Criminal->m_StaticMesh->SetRelativeRotation(FMath::Lerp(currRot, desiredRot,0.05f * a_DeltaTime*80.0f));
		m_Criminal->m_StaticMesh->SetPhysicsAngularVelocityInDegrees(FMath::Lerp(m_Criminal->m_StaticMesh->GetPhysicsAngularVelocityInDegrees(),FVector::Zero(),0.2f * a_DeltaTime*80.0f));

		m_ArrivedAtNextPoint = false;
	}
	else
	{
		
		m_ArrivedAtNextPoint = true;
	}
	
	
}

FIntVector3 ACriminalController::ConvertFromWorldPosToGridPos(FVector a_WPos)
{
	//checking if resolution is set, preventing division by 0
	if (m_MeshGridAsset->m_NavMeshResolution != FVector{ 0,0,0 })
	{
		FVector gridvec = a_WPos - m_MeshGridAsset->m_NavMeshBounds.Min;

		gridvec.X = (int)(gridvec.X / m_MeshGridAsset->m_VoxelSize.X);
		gridvec.Y = (int)(gridvec.Y / m_MeshGridAsset->m_VoxelSize.Y);
		gridvec.Z = (int)(gridvec.Z / m_MeshGridAsset->m_VoxelSize.Z);

		//UE_LOG(LogTemp, Display, TEXT("Unclamped:%f,%f,%f"), gridvec.X, gridvec.Y, gridvec.Z);
		gridvec.X = FMath::Clamp(gridvec.X, 2, m_MeshGridAsset->m_NavMeshResolution.X - 2);
		gridvec.Y = FMath::Clamp(gridvec.Y, 2, m_MeshGridAsset->m_NavMeshResolution.Y - 2);
		gridvec.Z = FMath::Clamp(gridvec.Z, 2, m_MeshGridAsset->m_NavMeshResolution.Z - 2);
		//UE_LOG(LogTemp, Display, TEXT("Clamped:%f,%f,%f"), gridvec.X, gridvec.Y, gridvec.Z);

		return FIntVector3(gridvec.X, gridvec.Y, gridvec.Z);
	}


	return FIntVector3(0, 0, 0);
}


//first of all Iam very sorry for the function that will come next
//Iam aware that this is not done that clean, but it works and I sat hours on solving the problem fo picking a
//next destination to pathfind to
//what this does in a summary:
//shooting raycasts to a point that can be traversed to
//from that point shooting another iteration of raycasts till there is a point that is not blocked
//since the raycasts find a point in under 6 iterations almost every time, the while loops are okay here
//there are noticeable frame drops so no downsides

//the first idea was to iterate though the nav mesh array and just picking an element that is not blocked as a destination,
//however since the navmesh does not account for filled areas, the inner volume of buildings are considered unblocked, that why using
//raycast was the only way to ensure a traverseable path without getting stuck

FIntVector3 ACriminalController::CalculateRandomPosition(FVector a_CurrentPos)
{
	int resY = m_MeshGridAsset->m_NavMeshResolution.Y;
	int resZ = m_MeshGridAsset->m_NavMeshResolution.Z;
	
	FHitResult hit;
	FHitResult hit2;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	TArray<FHitResult> hits;

	int nextPosX = 0;int nextPosY = 0;int nextPosZ = 0;
	int nextPosX2 = 0;int nextPosY2 = 0;int nextPosZ2 = 0;
					
	FIntVector finalDestination(0, 0, 0);
	FIntVector betweenDestination;
	FVector startOfNextDestinationPath;

	FVector desiredDestination;

	bool calculatingDestination1 = true;

	while (calculatingDestination1)
	{

		nextPosX = FMath::RandBool() ? FMath::RandRange(a_CurrentPos.X - m_MinRandomRange.X, a_CurrentPos.X - m_MaxRandomRange.X) : FMath::RandRange(a_CurrentPos.X + m_MinRandomRange.X, a_CurrentPos.X + m_MaxRandomRange.X);
		nextPosY = FMath::RandBool() ? FMath::RandRange(a_CurrentPos.Y - m_MinRandomRange.Y, a_CurrentPos.Y - m_MaxRandomRange.Y) : FMath::RandRange(a_CurrentPos.Y + m_MinRandomRange.Y, a_CurrentPos.Y + m_MaxRandomRange.Y);
		nextPosZ = FMath::RandBool() ? FMath::RandRange(a_CurrentPos.Z - m_MinRandomRange.Z, a_CurrentPos.Z - m_MaxRandomRange.Z) : FMath::RandRange(a_CurrentPos.Z + m_MinRandomRange.Z, a_CurrentPos.Z + m_MaxRandomRange.Z);
		desiredDestination = FVector(nextPosX, nextPosY, nextPosZ);

		//UE_LOG(LogTemp, Display, TEXT("DesiredDest:%f,%f,%f"), desiredDestination.X, desiredDestination.Y, desiredDestination.Z);
		GetWorld()->LineTraceSingleByChannel(hit, a_CurrentPos, desiredDestination, ECC_Visibility, QueryParams);
		if (hit.bBlockingHit)
		{
			//DrawDebugLineTraceSingle(GetWorld(), a_CurrentPos, hit.Location, EDrawDebugTrace::Type::ForDuration, true, hit, FLinearColor::Yellow, FLinearColor::Yellow, 5);
			//UE_LOG(LogTemp, Display, TEXT("Hit:%f,%f,%f"), hit.Location.X, hit.Location.Y, hit.Location.Z);

			continue;

		}
		betweenDestination = ConvertFromWorldPosToGridPos(desiredDestination);


		if(!m_MeshGridAsset->m_Grid[betweenDestination.X * resY * resZ + betweenDestination.Y * resZ + betweenDestination.Z])
		{
			//DrawDebugLineTraceSingle(GetWorld(), a_CurrentPos, desiredDestination, EDrawDebugTrace::Type::ForDuration, true, hit, FLinearColor::Blue, FLinearColor::Blue, 5);
			//UE_LOG(LogTemp, Display, TEXT("NoHit:%f,%f,%f"), desiredDestination.X, desiredDestination.Y, desiredDestination.Z);
			startOfNextDestinationPath = desiredDestination;
			calculatingDestination1 = false;
		}

	}
	

	FVector desiredDestination2;
	bool calculatingDestination2 = true;


	while (calculatingDestination2) {

		nextPosX2 = FMath::RandBool() ? FMath::RandRange(nextPosX - m_MinRandomRange.X, nextPosX - m_MaxRandomRange.X) : FMath::RandRange(nextPosX + m_MinRandomRange.X, nextPosX + m_MaxRandomRange.X);
		nextPosY2 = FMath::RandBool() ? FMath::RandRange(nextPosY - m_MinRandomRange.Y, nextPosY - m_MaxRandomRange.Y) : FMath::RandRange(nextPosY + m_MinRandomRange.Y, nextPosY + m_MaxRandomRange.Y);
		nextPosZ2 = FMath::RandBool() ? FMath::RandRange(nextPosZ - m_MinRandomRange.Z, nextPosZ - m_MaxRandomRange.Z) : FMath::RandRange(nextPosZ + m_MinRandomRange.Z, nextPosZ + m_MaxRandomRange.Z);

		if(nextPosZ2>= m_AltitudeBiasCap) nextPosZ2 = CalculateAltitudeWithBias(nextPosZ2);

		desiredDestination2 = FVector(nextPosX2, nextPosY2, nextPosZ2);

		GetWorld()->LineTraceSingleByChannel(hit2, startOfNextDestinationPath, desiredDestination2, ECC_Visibility, QueryParams);

		if (hit2.bBlockingHit)
		{
			//UE_LOG(LogTemp, Display, TEXT("Hit2:%f,%f,%f"), hit2.Location.X, hit2.Location.Y, hit2.Location.Z);
			//DrawDebugLineTraceSingle(GetWorld(), startOfNextDestinationPath, hit2.Location, EDrawDebugTrace::Type::ForDuration, true, hit, FLinearColor::Red, FLinearColor::Red, 5);
			continue;
		}
		finalDestination = ConvertFromWorldPosToGridPos(desiredDestination2);

		if(!m_MeshGridAsset->m_Grid[finalDestination.X * resY * resZ + finalDestination.Y * resZ + finalDestination.Z])
		{
			//UE_LOG(LogTemp, Display, TEXT("NoHit2:%f,%f,%f"), desiredDestination2.X, desiredDestination2.Y, desiredDestination2.Z);
			//DrawDebugLineTraceSingle(GetWorld(), startOfNextDestinationPath, desiredDestination2, EDrawDebugTrace::Type::ForDuration, true, hit, FLinearColor::Green, FLinearColor::Red, 5);

			calculatingDestination2 = false;
		}

	}
	
	//UE_LOG(LogTemp, Display, TEXT("Final Destination:%d,%d,%d"), finalDestination.X, finalDestination.Y, finalDestination.Z);
	return finalDestination;
}

//this handles the altitude of the criminal and preventing it to fly to high
int ACriminalController::CalculateAltitudeWithBias(float a_AltitudeToEvaluate)
{
	//this stuff is explained in header
	int random = FMath::RandRange(0, 100);

	if(random < m_KeepNewAltitudeChance) 
	{
		return a_AltitudeToEvaluate;
	}
	if(random >= m_KeepNewAltitudeChance&& random < m_SubtractHeightChance)
	{
		return a_AltitudeToEvaluate - FMath::RandRange(m_AltitudeMinSubractionValue, m_AltitudeMaxSubractionValue);
	}
	if(random >=m_SubtractHeightChance && random < m_TakeCappedAltitudeChance)
	{
		return m_AltitudeBiasCap;
	}
	if(random >= m_TakeCappedAltitudeChance && random <= m_KeepOldAltitudeChance)
	{
		return m_Criminal->m_StaticMesh->GetComponentLocation().Z;
	}
	//should actually never occur 
	return m_Criminal->m_StaticMesh->GetComponentLocation().Z;
}




