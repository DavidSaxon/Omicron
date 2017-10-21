#include <omicron/api/GameInterface.hpp>
#include <omicron/api/res/ResourceId.hpp>
#include <omicron/api/scene/Entity.hpp>
#include <omicron/api/scene/SceneState.hpp>
#include <omicron/api/scene/component/renderable/Mesh.hpp>

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
        // TODO: add component
        add_component(
            new omi::scene::Mesh(omi::res::get_id("res/builtin/mesh/bunny.obj"))
        );
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
        static bool do_once = true;
        if(do_once)
        {
            omi::scene::SceneState::instance().new_entity("hell_Enemy");
            do_once = false;
        }
    }
};

} // namespace hell

OMI_GAME_REGISTER_ENTITY(hell::ProtoEntity, hell_ProtoEntity);
OMI_GAME_REGISTER_ENTITY(hell::Player, hell_Player);
