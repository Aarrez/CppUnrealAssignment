#include "CoolDrone.h"
#include "Components/SphereComponent.h"

ACoolDrone::ACoolDrone()
{
	//Todo Spawn in sphere collider instead of trying to get it
	/*auto children = GetOwner()->Children;
	for(auto child: children)
	{
		if(child->GetClass()->IsChildOf(USphereComponent::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found SphereCollider"));
			TriggerSphere = Cast<USphereComponent>(child);
		}
	}*/
	PrimaryActorTick.bCanEverTick = true;
}

void ACoolDrone::BeginPlay()
{
	Super::BeginPlay();
}

void ACoolDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



