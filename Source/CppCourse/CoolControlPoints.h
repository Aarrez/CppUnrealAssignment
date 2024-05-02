#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoolControlPoints.generated.h"

class FAssetRegistryModule;

UCLASS()
class CPPCOURSE_API ACoolControlPoints : public AActor
{
	GENERATED_BODY()

	ACoolControlPoints();
public:
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");

	UPROPERTY(EditAnywhere)
	FString DefaultControlPointName = "DefaultControlPoint";
	
	
};
