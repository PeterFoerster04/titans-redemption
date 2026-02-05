// Fill out your copyright notice in the Description page of Project Settings.


#include "Criminal.h"

#include "Aircraft.h"
#include "GunProjectile.h"
#include "CriminalController.h"
#include "MathUtil.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/AllowWindowsPlatformTypes.h"

// Sets default values
ACriminal::ACriminal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(m_DefaultRoot);

	m_Controller = Cast<ACriminalController>(GetController());

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_StaticMesh->SetupAttachment(RootComponent);

	m_CriminalStats = CreateDefaultSubobject<UCriminalStats>(TEXT("Stats"));

	m_Marker = CreateDefaultSubobject<UWidgetComponent>(TEXT("Marker"));
	m_Marker->SetupAttachment(m_StaticMesh);


}

// Called when the game starts or when spawned
void ACriminal::BeginPlay()
{
	Super::BeginPlay();
	m_StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ACriminal::OnBoxBeginOverlap);
	m_StaticMesh->OnComponentHit.AddDynamic(this, &ACriminal::OnHit);
}


void ACriminal::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//the criminal can take multiple types of damage
	//hp
	//shields
	//complete shut down by emp
	//the shield is first consumed when hit and then the hp
	AGunProjectile* projectile = Cast<AGunProjectile>(OtherActor);
	AEMPProjectile* empblast = Cast<AEMPProjectile>(OtherActor);
	if( projectile!=nullptr)
	{
		if(m_CriminalStats->m_VehicleCurrentShields ==0&&!projectile->m_IsStun)
		{
			//UE_LOG(LogTemp, Display, TEXT("Criminal Take Damage"))
			TakeProjectileDamage(projectile->m_Damage,&m_CriminalStats->m_VehicleCurrentHP);
			UGameplayStatics::PlaySound2D(GetWorld(), m_HitSound);
		}
		else if(m_CriminalStats->m_VehicleCurrentShields > 0 && !projectile->m_IsStun)
		{
			//UE_LOG(LogTemp, Display, TEXT("Criminal Take Shield Damage"))
			TakeProjectileDamage(projectile->m_Damage, &m_CriminalStats->m_VehicleCurrentShields);
			UGameplayStatics::PlaySound2D(GetWorld(), m_ShieldHitSound);
		}
		else if(m_CriminalStats->m_VehicleCurrentShields ==0&&projectile->m_IsStun)
		{
			m_CriminalStats->m_SpeedMultiplier *= m_StunNewSpeedMultiply;
		}
		if(empblast== nullptr)
		{
			//only destroying if it is not emp blast
			projectile->Destroy();
		}
		
	}
	
}

void ACriminal::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//AAircraft* playerVehcile = Cast<AAircraft>(OtherActor);
	//if(playerVehcile != nullptr)
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Criminal Player Vehicle Crash"))
	//	//TakeProjectileDamage(30.0f, &m_CriminalStats->m_VehicleCurrentHP);
	//}
	
}

void ACriminal::TakeProjectileDamage(float a_Damage, float* a_ValueToModify)
{
	if (*a_ValueToModify> 0)
	{
		*a_ValueToModify -= a_Damage;
	}
	if(*a_ValueToModify <0)
	{
		*a_ValueToModify = 0;
	}
	//UE_LOG(LogTemp, Display, TEXT("CurrentHP:%f"), *a_ValueToModify)
}


// Called every frame
void ACriminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateKmpH(DeltaTime);
	
	//if the criminal was hit by emp the speed multiplier turns to 0
	//letting the criminal fall when that happens
	if(m_CriminalStats->m_SpeedMultiplier ==0.0f)
	{
		m_StaticMesh->SetEnableGravity(true);
	}
}

// Called to bind functionality to input
void ACriminal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACriminal::TakeCriticalDamage(float a_Damage)
{
	//this function is called when the criminal crashes into the player
	//unlike the projectile damage function, this bypasses the shields
	//and allows the player to slow down the vehicle on another way
	if(m_CriminalStats->m_VehicleCurrentHP > 0)
	{
		m_CriminalStats->m_VehicleCurrentHP -= a_Damage;
	}
	if(m_CriminalStats->m_VehicleCurrentHP < 0)
	{
		m_CriminalStats->m_VehicleCurrentHP = 0.0f;
	}
	//50% chance if vehicle slows down
	if(FMath::RandBool())
	{
		m_CriminalStats->m_SpeedMultiplier *= m_HitNewSpeedMultiply;
		UE_LOG(LogTemp, Display, TEXT("Criminal Speed Decreased"))
	}
}

void ACriminal::CalculateKmpH(float a_DeltaTime)
{
	float currVelocity = (m_StaticMesh->GetPhysicsLinearVelocity().Length()) * 0.036f;
	//smooth out the changing text
	m_CalculatedSpeedInKmph = FMathf::Lerp(m_CalculatedSpeedInKmph, currVelocity, 0.08f);
}


