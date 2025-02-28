#pragma once
#include "Actor.h"
#include "SphereActor.h"

class TargetPoint :
    public Actor
{
public:
    TargetPoint();
    ~TargetPoint();

    void setNewLocation();

private:
    SphereActor* sphere;

};

