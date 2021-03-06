// Jo's Work

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	//-1 is max down and +1 is max 
	void Elevate (float RelativeSpeed);

private:
	
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxDegreesPerSecond = 10.f;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevation = 0.f;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevation = 40.f;
};
