// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController_1.h" 

void ATankPlayerController_1::BeginPlay()
{
    Super::BeginPlay();

    auto ControlledTank = GetControlledTank();
    if(!ControlledTank)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayController not possesing a tank!"));
    }
    else 
    {
    UE_LOG(LogTemp, Warning, TEXT("PlayerController possesing %s."), *ControlledTank->GetName());
    }
}

void ATankPlayerController_1::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    AimTowardsCrossHair();
}

ATank* ATankPlayerController_1::GetControlledTank() const
{
    return Cast<ATank> (GetPawn());
}

void ATankPlayerController_1::AimTowardsCrossHair() 
{
    if (!GetControlledTank()) {return;}

    FVector HitLocation; //Out Parameter
    if (GetSightRayHitLocation(HitLocation))//Has side effect its going to line trace.
    {
        GetControlledTank()->AimAt(HitLocation);
        //tell control tank to aim at this point
    }
}

//Get world location of line trace through cross hair, true if hits landscape
bool ATankPlayerController_1::GetSightRayHitLocation(FVector& HitLocation) const
{
    //find the cross hair position
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);

    auto ScreenLocation = FVector2D (ViewportSizeX*CrossHairXLocation, ViewportSizeY*CrossHairYLocation);

    //De-project the screen position of the crosshair to a world directions
    FVector LookDirection;
    if(GetLookDirection(ScreenLocation, LookDirection))
    {
        //Line trace along that Look direction and see what we hit (up to max range)
        GetLookVectorHitLocation(LookDirection, HitLocation);


        //UE_LOG(LogTemp, Warning, TEXT("Look Direction: %s"), *LookDirection.ToString());
    }
    return true;
}

bool ATankPlayerController_1::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
    FVector CameraWorldLocation;
    return DeprojectScreenPositionToWorld(
        ScreenLocation.X, 
        ScreenLocation.Y, 
        CameraWorldLocation, 
        LookDirection
        );
}

bool ATankPlayerController_1::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
    FHitResult HitResult;
    auto StartLocation = PlayerCameraManager->GetCameraLocation();
    auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

    if(GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECollisionChannel::ECC_Visibility)
        )
    {
        HitLocation = HitResult.Location;
        return true;
    }

    HitLocation = FVector (0);
    return false;
}

