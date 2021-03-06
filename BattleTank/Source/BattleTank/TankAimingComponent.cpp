// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h" //whole include file is required as we are calling in a method from this code. therefore a forward declaration is not enough
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"
#include "TankAimingComponent.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) 
{
	if(!Barrel){return;}
	if(!Turret) {return;}

	FVector LaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this, 
		LaunchVelocity, 
		StartLocation, 
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	
	if(bHaveAimSolution)
	{
		auto TankName = GetOwner()->GetName();
		auto AimDirection = LaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection, LaunchVelocity);
		auto Time = GetWorld()->GetTimeSeconds();
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
    	UE_LOG(LogTemp, Error, TEXT("%f: No Aim Solution"), Time);
	}
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet) 
{
	if (!BarrelToSet){return;}
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet) 
{
	if (!TurretToSet){return;}
	Turret = TurretToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection, FVector LaunchVelocity) 
{
	//Work Out difference between current barrel rotation and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw); 
}

