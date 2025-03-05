#pragma once
#include "State.h"
#include "MoveComponent.h"

class StateMoveToTargetPoint :
    public State
{
public:
    void enter(Actor* bot) override;
    void update(Actor* bot, float deltaTime) override;
    void exit(Actor* bot) override;

    int getPriority() override;

private:
    MoveComponent* mc;
};

