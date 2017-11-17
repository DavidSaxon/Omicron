#include <arcanecore/base/math/MathOperations.hpp>
#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>

#include <omicron/api/GameInterface.hpp>
#include <omicron/api/context/Surface.hpp>
#include <omicron/api/res/ResourceId.hpp>
#include <omicron/api/scene/Entity.hpp>
#include <omicron/api/scene/SceneState.hpp>
#include <omicron/api/scene/component/renderable/Camera.hpp>
#include <omicron/api/scene/component/renderable/Mesh.hpp>
#include <omicron/api/scene/component/transform/AxisAngleTransform.hpp>
#include <omicron/api/scene/component/transform/TranslateTransform.hpp>

#include "hellbound/HellboundGlobals.hpp"

// TODO: REMOVE ME
#include <iostream>


namespace hell
{

class ProtoEntity
    : public omi::scene::Entity
{
private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    arc::lx::Vector2i m_window_size;

    omi::scene::AxisAngleTransform* m_spin;
    omi::scene::AxisAngleTransform* m_tilt;
    omi::scene::TranslateTransform* m_zoom;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    ProtoEntity(
            const arc::str::UTF8String& name,
            const omi::Attribute& data)
        : omi::scene::Entity(name)
        , m_spin(new omi::scene::AxisAngleTransform(
            0.0F,
            arc::lx::Vector3f(0.0, 1.0F, 0.0F)
        ))
        , m_tilt(new omi::scene::AxisAngleTransform(
            arc::math::degrees_to_radians(-15.0F),
            arc::lx::Vector3f(1.0, 0.0F, 0.0F),
            m_spin
        ))
        , m_zoom(new omi::scene::TranslateTransform(
            arc::lx::Vector3f(0.0F, 0.0F, 3.0F),
            m_tilt
        ))
    {
        add_component(m_spin);
        add_component(m_tilt);
        add_component(m_zoom);

        // TODO:
        omi::scene::Camera* camera = new omi::scene::Camera(
            23.9276F,
            arc::lx::Vector2f(28.2478F, 14.9273F),
            arc::lx::Vector2f(0.0F, 0.0F),
            0.01F,
            10000.0F,
            m_zoom
        );
        add_component(camera);
        add_component(new omi::scene::Mesh(
            omi::res::get_id("res/builtin/mesh/shapes.obj")
        ));

        // set the active camera
        omi::scene::SceneState::instance().set_active_camera(camera);

        // event subscriptions
        subscribe_to_event(omi::context::Event::kTypeMouseMove);
        subscribe_to_event(omi::context::Event::kTypeMouseScroll);
        subscribe_to_event(omi::context::Event::kTypeKeyPress);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~ProtoEntity()
    {
    }

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void on_event(const omi::context::Event& event) override
    {
        arc::int32 scroll_amount_x = 0;
        arc::int32 scroll_amount_y = 0;
        omi::context::Event::KeyCode key_code;

        // TODO: utility for this
        if(event.get_type() == omi::context::Event::kTypeMouseMove)
        {
            // get the mouse position
            omi::Int32Attribute mouse_pos_attr =
                event.get_data()[omi::context::Event::kDataMousePosition];
            if(!mouse_pos_attr.is_valid() || mouse_pos_attr.get_size() != 2)
            {
                // TODO:
                return;
            }

            // get the mouse position
            arc::lx::Vector2i mouse_pos(
                mouse_pos_attr.get_values()[0],
                mouse_pos_attr.get_values()[1]
            );

            arc::int32 half_width =
                omi::context::Surface::instance()->get_width() / 2;
            arc::int32 half_height =
                omi::context::Surface::instance()->get_height() / 2;

            // move by the difference
            m_spin->angle() += (mouse_pos(0) - half_width) * -0.002F;
            m_tilt->angle() += (mouse_pos(1) - half_height) * -0.002F;
        }
        else if(omi::context::Event::mouse_scroll(
            event,
            scroll_amount_x,
            scroll_amount_y
        ))
        {
            m_zoom->translation()(2) -= scroll_amount_y * 0.1F;
        }
        else if(omi::context::Event::key_press(event, key_code))
        {
            if(key_code == omi::context::Event::kKeyCodeF)
            {
                m_spin->angle() = 0.0F;
                m_tilt->angle() = 0.0F;
            }
        }
    }

    virtual void update() override
    {
        // rotate the camera
        // m_spin->angle() += arc::math::degrees_to_radians(0.25F);
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
