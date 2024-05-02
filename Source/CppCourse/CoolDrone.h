#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoolDrone.generated.h"

class USphereComponent;
class UCoolMovementComponent;
class UCoolHealthComponent;
class ATurretProjectile;
UCLASS()
class CPPCOURSE_API ACoolDrone : public AActor
{
	GENERATED_BODY()

protected:
	ACoolDrone();
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UFUNCTION()
	void HandleDeath();
	
	FVector GetDirectionToClosestControlPoint(TArray<AActor*>& _ControlPoints);

	void MoveDrone(const float& _DeltaSeconds);

	void StopAndShoot(const float& _DeltaSeconds);

	void SetGoal();

	void DroneShoot(const float& _DeltaSeconds);
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* ShootRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
	AActor* Target = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ControlPoints;

	UPROPERTY(EditAnywhere)
	bool UseDefaultControlPoints = true;

	UPROPERTY(VisibleAnywhere)
	bool EnemySpotted = false;

	UPROPERTY(EditAnywhere)
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere)
	float Speed = 10.0f;

	UPROPERTY(EditAnywhere)
	float RotationModifier = 0.01f;

	UPROPERTY(VisibleAnywhere)
	float CurrentDistanceFromTarget;

	UPROPERTY(EditDefaultsOnly)
	float AcceptedDistanceFromPoint = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATurretProjectile> ProjectileClass;

private:

	UPROPERTY()
	UCoolHealthComponent* HealthComponent = nullptr;
	
	float RotationAlpha = 0.f;

	float LastFireTime = 0.f;
	
	FVector GoalDirection;
	FVector GoalVector;
	FRotator TargetLookRotator;
	FVector TargetDirection;
};
