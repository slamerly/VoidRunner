#pragma once
#include "Actor.h"
#include "MeshComponent.h"


class Station :
    public Actor
{
public:
    Station(int meshNumber = 1);
    ~Station();

private:
    MeshComponent* mc;

};

