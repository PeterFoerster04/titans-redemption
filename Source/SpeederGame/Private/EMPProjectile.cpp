// Fill out your copyright notice in the Description page of Project Settings.


#include "EMPProjectile.h"

#include "Aircraft.h"
#include "Criminal.h"
#include "CriminalController.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AEMPProjectile::AEMPProjectile()
{

	m_EMPNiagara = CreateDefaultSubobject<UNiagaraComponent>("EMPSystem");
	m_EMPNiagara->SetupAttachment(RootComponent);

	m_TriggerSphere = CreateDefaultSubobject<USphereComponent>("Trigger");
	m_TriggerSphere->SetupAttachment(RootComponent);
}
//checking if the bomb was detonated before applying the effects
void AEMPProjectile::OnBlastBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACriminal* criminal = Cast<ACriminal>(OtherActor);
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && criminal != nullptr)
	{
		//making criminal not move if hit
		criminal->m_CriminalStats->m_SpeedMultiplier = 0.0f;
		criminal->m_CriminalStats->m_VehicleCurrentShields = 0.0f;
		criminal->m_WasEMPd = true;
		ACriminalController* controller = Cast<ACriminalController>(criminal->GetController());
		controller->m_Move = false;
		criminal->m_StaticMesh->SetEnableGravity(true);
		//criminal->m_StaticMesh->AddImpulse(FVector::DownVector * 1000.0f, NAME_None);
		UE_LOG(LogTemp,Display,TEXT("Hit Criminal"))
		
	}
	
}
//detonating if the bomb hits something
void AEMPProjectile::OnBombCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (m_AlreadyDetonated) return;
	AAircraft* player = Cast<AAircraft>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn());
	if (player != nullptr) player->ExplosionCamShake();
	Detonate();
	m_AlreadyDetonated = true;
}

void AEMPProjectile::BeginPlay()
{
	Super::BeginPlay();
	m_TriggerSphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	m_TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEMPProjectile::OnBlastBeginOverlap);
	m_ProjectileMesh->OnComponentHit.AddDynamic(this, &AEMPProjectile::OnBombCompHit);
	
}

void AEMPProjectile::Detonate()
{
	m_EMPNiagara->SetActive(true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_BlastSound, GetActorLocation(), GetActorRotation());
	m_ProjectileMesh->SetVisibility(false);
	m_TriggerSphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}
