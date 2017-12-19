#include "ScoreComponent.h"
#include <iostream>

ScoreComponent::ScoreComponent(GameObject& newGameObject) : IComponent(newGameObject)
{
    lap=1;
    position=1;
    maxWaypoints=300;
}

//Maximum number of waypoints in a lap is set in the variable : maxWaypoints
int ScoreComponent::getScore()
{
    auto pComp = this->getGameObject().getComponent<PathPlanningComponent>().get();

    int id = this->getGameObject().getId();

    std::cout << "ID: " << std::endl;

    std::cout << id << ": " << (lap*maxWaypoints)+pComp->getActualLevel() << std::endl;

    return (lap*maxWaypoints)+pComp->getActualLevel();
}

float ScoreComponent::getActualDistance()
{
    auto pComp = this->getGameObject().getComponent<PathPlanningComponent>().get();

    return pComp->getActualDistance();
}