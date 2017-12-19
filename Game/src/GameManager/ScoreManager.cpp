#include "ScoreManager.h"

ScoreManager::ScoreManager()
{

}

ScoreManager::~ScoreManager()
{

}

void ScoreManager::init()
{
    
}

//Thirty programmers have died during the development of this method
//Mejoras : Quitar medio metodo, que se puede hacer en el mismo for todo
void ScoreManager::update()
{
    std::vector<ScoreComponent::Pointer> ordered;
    std::vector<ScoreComponent::Pointer> auxiliar;
    uint32_t i, j, pos, ordCount;
    int score;
    bool found;

    pos=1;
    for(i=0; i<players.size(); i++)
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
                if(score>ordered[j].get()->getScore())
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
    ordCount=0;
    for(i=0; i<ordered.size()-1; i++)
    {
        if(ordered[i].get()->getScore() == ordered[i+1].get()->getScore())
        {
            ordCount++;
            if(ordCount==1)
            {
                auxiliar.push_back(ordered[i]);
            }
            else
            {
                found=false;
                for(j=0; j<auxiliar.size() && found==false; j++)
                {
                    if(ordered[i].get()->getActualDistance() < auxiliar[j].get()->getActualDistance())
                    {
                        auxiliar.insert(auxiliar.begin()+j, ordered[i]);
                        found=true;
                    }
                }
                if(found==false)
                {
                    auxiliar.push_back(ordered[i]);
                }
            }
        }
        else
        {
            if(ordCount>0)
            {
                found = false;
                for(j=0; j<auxiliar.size() && found==false; j++)
                {
                    if(ordered[i].get()->getActualDistance() < auxiliar[j].get()->getActualDistance())
                    {
                        auxiliar.insert(auxiliar.begin()+j, ordered[i]);
                        found=true;
                    }
                }
                if(found == false)
                {
                    auxiliar.push_back(ordered[i]);
                }
                ordered.erase(ordered.begin()+(i-ordCount), ordered.begin()+i);
                for(j=auxiliar.size()-1;j>=0; j--)
                {
                    ordered.insert(ordered.begin()+i, auxiliar[j]);
                }
                auxiliar.clear();
            }
        }

    }

    for(i=0; i<ordered.size(); i++)
    {
        ordered[i].get()->setPosition(pos);
        pos++;
    }
    players=ordered;

}