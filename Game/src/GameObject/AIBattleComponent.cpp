#include "AIBattleComponent.h"


//This class defines the usage of a generic item in the Battle System
class UseItemAction : public Behaviour{


};

//This class defines the waiting action, when the player decides not to use the item
class WaitAction : public Behaviour{

};

//Condition that checks if the player is first or not
class ConditionFirstAction : public Behaviour {

};

//Condition that checks if an enemy is behind or not
class ConditionEnemyBehindAction : public Behaviour {

};

//Condition that checks if the player has an item or not
class ConditionHasItemAction : public Behaviour {

};

//Condition that checks if the player has the item 1 or not
class ConditionHasItem1Action : public Behaviour {

};

//Condition that checks if the player has the item 2 or not
class ConditionHasItem2Action : public Behaviour {

};

//Condition that checks if the player has the item 3 or not
class ConditionHasItem3Action : public Behaviour {

};

//Condition that checks if the player has the item 4 or not
class ConditionHasItem4Action : public Behaviour {

};

//Condition that checks if the player has the item 5 or not
class ConditionHasItem5Action : public Behaviour {

};

//Initialize the tree
void AIBattleComponent::init()
{
    //Selector is already declared in the header
    selector = std::shared_ptr<Selector>(new Selector());

    //Initialice all the nodes
    auto sequence = std::shared_ptr<Sequence>(new Sequence());

    auto selector2 = std::shared_ptr>Selector>(new Selector());

    auto sequence1 = std::shared_ptr<Sequence>(new Sequence());
    auto sequence2 = std::shared_ptr<Sequence>(new Sequence());
    auto sequence3 = std::shared_ptr<Sequence>(new Sequence());
    auto sequence4 = std::shared_ptr<Sequence>(new Sequence());
    auto sequence5 = std::shared_ptr<Sequence>(new Sequence());

    auto DS2 = std::shared_ptr<DynamicSelector>(new DynamicSelector());
    auto DS3 = std::shared_ptr<DynamicSelector>(new DynamicSelector());

    auto sequence21 = std::shared_ptr<Sequence>(new Sequence());
    auto sequence31 = std::shared_ptr<Sequence>(new Sequence());

    //We seet the root as a Repeat, and his only child is the main selector
    root = std::shared_ptr<Behaviour>(new Repeat(selector));

    //First selector. Chooses between having an object or not. If not, Waits.
    selector->addChild(sequence);
    selector->addChild(std::shared_ptr<Behaviour>(new WaitAction()));

    //Checks if we have an item, if not, we go back and wait is activated. If we have it, we active selector2
    sequence->addChild(std::shared_ptr<Behaviour>(new ConditionHasItemAction()));
    sequence->addChild(selector2);

    //Fice sequences, one for every kind of item
    selector2->addChild(sequence1);
    selector2->addChild(sequence2);
    selector2->addChild(sequence3);
    selector2->addChild(sequence4);
    selector2->addChild(sequence5);

    //Sequence for item 1: Blue shell. If we have it, use it
    sequence1->addChild(std::shared_ptr<Behaviour>(new ConditionHasItem1Action()));
    sequence1->addChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    //Sequence for item 2: Red Shell. If we have it, we enter in it's correspondant Dynamic Selector
    sequence2->addChild(std::shared_ptr<Behaviour>(new ConditionHasItem2Action()));
    sequence2->addChild(DS2);

    //Sequence for item 3: Banana. If we have it, we enter in it's correspondant Dynamic Selector
    sequence3->addChild(std::shared_ptr<Behaviour>(new ConditionHasItem3Action()));
    sequence3->addChild(DS3);

    //Sequence for item 4: Mushroom. If we have it, use it
    sequence4->addChild(std::shared_ptr<Behaviour>(new ConditionHasItem4Action()));
    sequence4->addChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    //Sequence for item 5: Star. If we have it, use it
    sequence5->addChild(std::shared_ptr<Behaviour>(new ConditionHasItem5Action()));
    sequence5->addChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    //Dynamic selector of item 2. If we are first, wait, elsewhere, use the item
    DS2->addChild(sequence21);
    DS2->addChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    sequence21->addChild(std::shared_ptr<Behaviour>(new ConditionFirstAction));
    sequence21->addChild(std::shared_ptr<Behaviour>(new WaitAction()));

    //Dynamic selector of item 3. If we have an enemy behind, use it, elsewhere wait
    DS3->addChild(sequence31);
    DS3->addChild(std::shared_ptr<Behaviour>(new WaitAction()));

    sequence31->addChild(std::shared_ptr<Behaviour>(new ConditionEnemyBehindAction()));
    sequence31->addChild(std::shared_ptr<Behaviour>(new UseItemAction()));


}

void AIBattleComponent::update(float dTime)
{

}

void AIBattleComponent::close()
{

}
