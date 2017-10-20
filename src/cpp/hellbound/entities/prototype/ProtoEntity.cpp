#include <omicron/api/GameInterface.hpp>
#include <omicron/api/scene/Entity.hpp>

#include "hellbound/HellboundGlobals.hpp"


namespace hell
{

class ProtoEntity
    : public omi::scene::Entity
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    ProtoEntity(
            const arc::str::UTF8String& name,
            const omi::Attribute& data)
        : omi::scene::Entity(name)
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~ProtoEntity()
    {
    }

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void update() override
    {
        global::logger->notice << "PROTO ENTITY!" << std::endl;
    }
};

class Player
    : public omi::scene::Entity
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    Player(
            const arc::str::UTF8String& name,
            const omi::Attribute& data)
        : omi::scene::Entity(name)
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Player()
    {
    }

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void update() override
    {
        global::logger->notice << "Player!" << std::endl;
    }
};

} // namespace hell

OMI_GAME_REGISTER_ENTITY(hell::ProtoEntity, hell_ProtoEntity);
OMI_GAME_REGISTER_ENTITY(hell::Player, hell_Player);
