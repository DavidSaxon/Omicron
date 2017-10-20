#include <omicron/api/GameInterface.hpp>
#include <omicron/api/scene/Entity.hpp>

#include "hellbound/HellboundGlobals.hpp"


namespace hell
{

class Enemy
    : public omi::scene::Entity
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    Enemy(
            const arc::str::UTF8String& name,
            const omi::Attribute& data)
        : omi::scene::Entity(name)
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Enemy()
    {
    }

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void update() override
    {
        global::logger->notice << "Enemy!" << std::endl;
    }
};

} // namespace hell

OMI_GAME_REGISTER_ENTITY(hell::Enemy, hell_Enemy);
