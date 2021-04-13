// Jo's Work

#include "BattleTank.h"
#include "TankTurret.h"


void UTankTurret::Rotate(float RelativeSpeed) 
{
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
    auto RotationChange = RelativeSpeed * MovementSpeedPerSecond* GetWorld()->DeltaTimeSeconds;
    auto Rotation = RelativeRotation.Yaw + RotationChange;
    SetRelativeRotation(FRotator(0.f, Rotation, 0.f));
}
