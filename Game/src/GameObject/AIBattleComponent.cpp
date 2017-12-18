#include "AIBattleComponent.h"


//This class defines the usage of a generic item in the Battle System
class UseItemAction : public Behaviour{
    protected:
        BehaviourState Update(float d)
        {
			return BehaviourState::FAILED;
        }

};

//This class defines the waiting action, when the player decides not to use the item
class WaitAction : public Behaviour{
    public:
        WaitAction(float w) :
        wTime(w) {}
        
    protected:
        void Initialized() override
        {
            aTime=0.f;
        }
        BehaviourState Update(float d)
        {
            aTime+=d;
            if(aTime>wTime)
            {
                return BehaviourState::SUCCEEDED;
            }
			return BehaviourState::RUNNING;
        }

    private:
        //Total time to wait
        float wTime;
        //Acumulated time
        float aTime;
};

//Condition that checks if the player is first or not
class ConditionFirstAction : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if an enemy is behind or not
class ConditionEnemyBehindAction : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if the player has an item or not
class ConditionHasItemAction : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if the player has the item 1 or not
class ConditionHasItem1Action : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if the player has the item 2 or not
class ConditionHasItem2Action : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if the player has the item 3 or not
class ConditionHasItem3Action : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if the player has the item 4 or not
class ConditionHasItem4Action : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Condition that checks if the player has the item 5 or not
class ConditionHasItem5Action : public Behaviour {
    protected:
        BehaviourState Update(float d)
        {
            return BehaviourState::FAILED;
        }

};

//Initialize the tree
void AIBattleComponent::init()
{
    //Main selector of the tree
    auto selector = std::shared_ptr<Selector>(new Selector());

    //Initialice all the nodes
    auto sequence = std::shared_ptr<Sequence>(new Sequence());

    auto selector2 = std::shared_ptr<Selector>(new Selector());

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
    selector->AddChild(sequence);
    selector->AddChild(std::shared_ptr<Behaviour>(new WaitAction(1.0f)));

    //Checks if we have an item, if not, we go back and wait is activated. If we have it, we active selector2
    sequence->AddChild(std::shared_ptr<Behaviour>(new ConditionHasItemAction()));
    sequence->AddChild(selector2);

    //Fice sequences, one for every kind of item
    selector2->AddChild(sequence1);
    selector2->AddChild(sequence2);
    selector2->AddChild(sequence3);
    selector2->AddChild(sequence4);
    selector2->AddChild(sequence5);

    //Sequence for item 1: Blue shell. If we have it, use it
    sequence1->AddChild(std::shared_ptr<Behaviour>(new ConditionHasItem1Action()));
    sequence1->AddChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    //Sequence for item 2: Red Shell. If we have it, we enter in it's correspondant Dynamic Selector
    sequence2->AddChild(std::shared_ptr<Behaviour>(new ConditionHasItem2Action()));
    sequence2->AddChild(DS2);

    //Sequence for item 3: Banana. If we have it, we enter in it's correspondant Dynamic Selector
    sequence3->AddChild(std::shared_ptr<Behaviour>(new ConditionHasItem3Action()));
    sequence3->AddChild(DS3);

    //Sequence for item 4: Mushroom. If we have it, use it
    sequence4->AddChild(std::shared_ptr<Behaviour>(new ConditionHasItem4Action()));
    sequence4->AddChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    //Sequence for item 5: Star. If we have it, use it
    sequence5->AddChild(std::shared_ptr<Behaviour>(new ConditionHasItem5Action()));
    sequence5->AddChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    //Dynamic selector of item 2. If we are first, wait, elsewhere, use the item
    DS2->AddChild(sequence21);
    DS2->AddChild(std::shared_ptr<Behaviour>(new UseItemAction()));

    sequence21->AddChild(std::shared_ptr<Behaviour>(new ConditionFirstAction));
    sequence21->AddChild(std::shared_ptr<Behaviour>(new WaitAction(1.0f)));

    //Dynamic selector of item 3. If we have an enemy behind, use it, elsewhere wait
    DS3->AddChild(sequence31);
    DS3->AddChild(std::shared_ptr<Behaviour>(new WaitAction(1.0f)));

    sequence31->AddChild(std::shared_ptr<Behaviour>(new ConditionEnemyBehindAction()));
    sequence31->AddChild(std::shared_ptr<Behaviour>(new UseItemAction()));
}

void AIBattleComponent::update(float dTime)
{
    root->Tick(dTime);

}

void AIBattleComponent::close()
{

}
