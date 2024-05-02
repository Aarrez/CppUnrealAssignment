#include "CoolDrone.h"

#include "CoolControlPoints.h"
#include "CoolHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretProjectile.h"

ACoolDrone::ACoolDrone()
{
	
	TriggerSphere = CreateDefaultSubobject<USphereComponent>("TriggerArea");
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Root->SetupAttachment(TriggerSphere);
	ShootRoot = CreateDefaultSubobject<USceneComponent>("ShootPoint");
	ShootRoot->SetupAttachment(Root);
	
	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoolDrone::HandleBeginOverlap);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ACoolDrone::HandleEndOverlap);

	HealthComponent = CreateDefaultSubobject<UCoolHealthComponent>("Health");
	HealthComponent->OnDeath.AddDynamic(this, &ACoolDrone::HandleDeath);
	
	PrimaryActorTick.bCanEverTick = true;
}

void ACoolDrone::BeginPlay()
{
	Super::BeginPlay();
	if(UseDefaultControlPoints)
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoolControlPoints::StaticClass(), ControlPoints);
	SetGoal();

	UE_LOG(LogTemp, Warning, TEXT("x:%f y:%f"), GoalVector.X, GoalVector.Y);
	/*GoalDirection = FVector(GoalDirection.X, GoalDirection.Y, 0);*/
}

void ACoolDrone::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Target = OtherActor;
	EnemySpotted = true;
	RotationAlpha = 0;
}

void ACoolDrone::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Target = nullptr;
	EnemySpotted = false;
	RotationAlpha = 0;
}

void ACoolDrone::HandleDeath()
{
	Destroy();
}

FVector ACoolDrone::GetDirectionToClosestControlPoint(TArray<AActor*>& _ControlPoints) 
{
	double ShortestDistance = 100000.0;
	int32 ShortestPath {};
	for (int i {0}; i < _ControlPoints.Num()-1; i++)
	{
		FVector TargetLocation = _ControlPoints[i]->GetActorLocation();
		auto distance = FVector::Distance(TargetLocation, GetActorLocation());
		
		if(ShortestDistance < distance) continue;
		ShortestDistance = distance;
		ShortestPath = i;
	}
	TArray<AActor*> TempArray;
	for(int i {0}; i < _ControlPoints.Num()-1; i++)
	{
		if(i == ShortestPath) continue;
		TempArray.Add(_ControlPoints[i]);
	}

	int randNum = FMath::RandRange(0, TempArray.Num()-1);

	FVector point = TempArray[randNum]->GetTargetLocation();
	UE_LOG(LogTemp, Warning, TEXT("%s Random number: %i"), *TempArray[randNum]->GetName(), randNum);
	
	return point;
}

void ACoolDrone::SetGoal()
{
	GoalDirection = GetDirectionToClosestControlPoint(ControlPoints);
	GoalVector = GoalDirection;
	GoalDirection = GoalDirection - GetActorLocation();
	GoalDirection = GoalDirection.GetSafeNormal2D();
	TargetLookRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GoalVector);
	RotationAlpha = 0;
}

void ACoolDrone::MoveDrone(const float& _DeltaSeconds)
{
	double distance = FVector::Distance(GetActorLocation(), GoalVector);
	FRotator some = FMath::Lerp(GetActorRotation(), TargetLookRotator, RotationAlpha);
	SetActorRotation(some);
	if(distance < AcceptedDistanceFromPoint)
		SetGoal();
	if(RotationAlpha <= 1)
		RotationAlpha += RotationModifier * _DeltaSeconds;
	
	SetActorRelativeLocation(GetActorLocation() + GoalDirection * _DeltaSeconds *  Speed);
}

void ACoolDrone::StopAndShoot(const float& _DeltaSeconds)
{
	TargetLookRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	FRotator some = FMath::Lerp(GetActorRotation(), TargetLookRotator, RotationAlpha);
	SetActorRotation(some);
	if(RotationAlpha <= 1)
		RotationAlpha += RotationModifier * _DeltaSeconds;
	DroneShoot(_DeltaSeconds);
}

void ACoolDrone::DroneShoot(const float &_DeltaSeconds)
{
	FVector _TargetDirection = Target->GetActorLocation() - GetActorLocation();
	_TargetDirection.Normalize();

	FVector CurrentDirection = GetActorForwardVector();

	float AimDotProduct = FVector::DotProduct(CurrentDirection, _TargetDirection);
	if (AimDotProduct > 0.98f)
	{
		if (GetWorld()->TimeSince(LastFireTime) > 1.f / FireRate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Drone Shoot"));
			GetWorld()->SpawnActor<ATurretProjectile>(
				ProjectileClass,
				ShootRoot->GetComponentLocation(),
				ShootRoot->GetComponentRotation()
			);
			LastFireTime = GetWorld()->GetTimeSeconds();
		}
	}
	else
	{
		LastFireTime = GetWorld()->GetTimeSeconds();
	}
}

void ACoolDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(EnemySpotted)
		StopAndShoot(DeltaSeconds);
	else
		MoveDrone(DeltaSeconds);
}



