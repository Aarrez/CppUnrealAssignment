#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoolDrone.generated.h"

class USphereComponent;

UCLASS()
class CPPCOURSE_API ACoolDrone : public AActor
{
	GENERATED_BODY()

public:
	ACoolDrone();

protected:
	virtual void BeginPlay() override;

	//Uncomment after implmeneted in CoolDrone.cpp
	/*UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	
public:
	virtual void Tick(float DeltaSeconds) override;

	//Spawn in instead of get
	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere = nullptr;
};
