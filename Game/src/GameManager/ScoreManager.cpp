#include "ScoreManager.h"

//==============================================
// FORWARD DECLARATIONS
//==============================================
void objectDeleteScore(EventData);


//==============================================
// MAIN FUNCTIONS
//==============================================
ScoreManager::ScoreManager()
{
    //Maximum number of laps
    //:::> Should be set in menu_state before the game runs
    maxLaps = 3;
}

ScoreManager::~ScoreManager()
{

}

void ScoreManager::init()
{
    //Bind listeners
    EventManager::getInstance().addListener(EventListener {EventType::GameObject_Delete, objectDeleteScore});
}

void ScoreManager::close()
{
    players.clear();
}

//Static getter
ScoreManager& ScoreManager::getInstance() {
    static ScoreManager instance;
    return instance;
}

//Component creator
IComponent::Pointer ScoreManager::createScoreComponent(GameObject& newGameObject){
    //Make shared pointer of the score component
    ScoreComponent::Pointer component = std::make_shared<ScoreComponent>(newGameObject);

    //Attach to game object
    newGameObject.addComponent(component);

    //Push it into the list of players
    //:::>Can  be substituted with an event in the future with scheduling
    players.push_back(component);

    return component;
}

IComponent::Pointer ScoreManager::createStartLineComponent(GameObject& newGameObject){
    //Make shared pointer of the start line component
    StartLineComponent::Pointer component = std::make_shared<StartLineComponent>(newGameObject);

    //Attach to game object
    newGameObject.addComponent(component);

    //Push it into the list of start lines
    //:::>Can  be substituted with an event in the future with scheduling
    startLines.push_back(component);

    return component;
}

//Thirty programmers have died during the development of this method
void ScoreManager::update()
{
    std::vector<ScoreComponent::Pointer> ordered;
    std::vector<ScoreComponent::Pointer> auxiliar;
    uint32_t j, pos;
    int score;
    bool found;

    pos=1;
    if(players.size()>0)
    {
        for(unsigned int i=0; i<players.size(); i++)
        {
            if(i==0)
            {
                ordered.push_back(players[i]);
            }
            else
            {
                found=false;
                score = players[i].get()->getScore();
                for(j=0; j<ordered.size() && found==false; j++)
                {
                    if((score>ordered[j].get()->getScore()) || (score == ordered[j].get()->getScore() && players[i].get()->getActualDistance() < ordered[j].get()->getActualDistance()))
                    {
                        ordered.insert(ordered.begin()+j, players[i]);
                        found=true;
                    }
                }
                if(found==false)
                {
                    ordered.push_back(players[i]);
                }
            }
        }
        for(unsigned int i=0; i<ordered.size(); i++)
        {
            ordered[i].get()->setPosition(pos);
            pos++;
        }
        players=ordered;

        for(unsigned int i=0; i<players.size(); i++)
        {
            //int p = players[i].get()->getPosition();
            //int id = players[i].get()->getGameObject().getId();
            //std::cout << id << ": " << p << std::endl;
        }
    }
}


//==============================================
// DELEGATES
//==============================================
void objectDeleteScore(EventData eData) {

    auto& scoreComponentList = ScoreManager::getInstance().getPlayers();

    for(unsigned int i = 0; i<scoreComponentList.size(); ++i) {
        if(eData.Id == scoreComponentList[i].get()->getGameObject().getId()) {
            scoreComponentList.erase(scoreComponentList.begin() + i);
            return;
        }
    }
}