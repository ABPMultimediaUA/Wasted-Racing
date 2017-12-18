 #pragma once

 #include "Behaviour.h"
 #include <vector>
 #include <memory>

 class Composite : public Behaviour 
 {
    protected:
        std::vector<Behaviour::Pointer> c_children;

    public:
        typedef std::shared_ptr<Composite> Pointer;

        template<class Behaviour>
        void AddChild(const std::shared_ptr<Behaviour>& child)
        {
            c_children.push_back(child);
        }

        virtual void Stop() override;
        virtual void Reset() override;

 };
