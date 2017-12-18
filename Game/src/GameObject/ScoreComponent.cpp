#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(GameObject& newGameObject) : IComponent(newGameObject)
{
    lap=1;
    position=1;
}

//Maximum number of waypoints in a lap : 300
int ScoreComponent::getScore()
{
    return 0;


}