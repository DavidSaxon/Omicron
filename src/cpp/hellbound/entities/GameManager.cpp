#include <omicron/api/GameInterface.hpp>
#include <omicron/api/context/EventService.hpp>
#include <omicron/api/scene/Entity.hpp>

#include "hellbound/HellboundGlobals.hpp"


namespace hell
{

// Entity which manages the game state of Hellbound
class GameManager
    : public omi::scene::Entity
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    GameManager(
            const arc::str::UTF8String& name,
            const omi::Attribute& data)
        : omi::scene::Entity(name)
    {
        // event subscriptions
        subscribe_to_event(omi::context::Event::kTypeKeyPress);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~GameManager()
    {
    }

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void on_event(const omi::context::Event& event) override
    {
        omi::context::Event::KeyCode key_code;
        if(omi::context::Event::key_press(event, key_code))
        {
            if(key_code == omi::context::Event::kKeyCodeEscape)
            {
                omi::context::EventService::instance().broadcast_shutdown();
            }
        }
    }

    virtual void update() override
    {
        // do nothing
    }
};

} // namespace hell

OMI_GAME_REGISTER_ENTITY(hell::GameManager, hell_GameManager);
