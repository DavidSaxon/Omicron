#include "omicron/api/scene/SceneState.hpp"

#include <cassert>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/api/render/RenderSubsystem.hpp"
#include "omicron/api/report/Logging.hpp"
#include "omicron/api/report/stats/StatsDatabase.hpp"
#include "omicron/api/scene/Entity.hpp"
#include "omicron/api/scene/SceneGlobals.hpp"
#include "omicron/api/scene/component/AbstractComponent.hpp"
#include "omicron/api/scene/component/renderable/AbstractRenderable.hpp"
#include "omicron/api/scene/component/renderable/Camera.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class SceneState::SceneStateImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //----------------------P R I V A T E    S T R U C T S----------------------

    struct EntityFactory
    {
        omi::GameEntityCreate* create_func;
        omi::GameEntityDestroy* destroy_func;
    };

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    /*!
     * \brief Mapping from entity identifier names to factory use to create and
     *        destroy them.
     */
    std::unordered_map<arc::str::UTF8String, EntityFactory*> m_factories;

    // the entities current within the scene
    std::list<Entity*> m_entities;

    // whether an update cycle is in progress or not
    bool m_in_update;
    // entities queue to be updated at the end of the current cycle
    std::list<Entity*> m_new_entities;

    // the camera the scene is being rendered through
    const omi::scene::Camera* m_active_camera;
    bool m_camera_changed;

    // stats
    omi::Int64Attribute m_stat_registered_entity_types;
    omi::Int64Attribute m_stat_active_entities;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SceneStateImpl()
        : m_in_update                   (false)
        , m_active_camera               (nullptr)
        , m_camera_changed              (false)
        , m_stat_registered_entity_types(0, false)
        , m_stat_active_entities        (0, false)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~SceneStateImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool startup_routine()
    {
        // create the logging profile
        arc::log::Profile profile("OMICRON-SCENE");
        // vend the input from the shared handler
        omi::scene::global::logger =
            omi::report::log_handler.vend_input(profile);

        global::logger->debug << "SceneState startup." << std::endl;

        // set up the stats
        omi::report::StatsDatabase::instance()->define_entry(
            "Scene.Registered Entity Types",
            m_stat_registered_entity_types,
            "The total number of entities that have been registered within "
            "this runtime."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "Scene.Active Entities",
            m_stat_active_entities,
            "The number of entity instances that are current active within the "
            "Omicron scene."
        );

        return true;
    }

    bool shutdown_routine()
    {
        global::logger->debug << "SceneState shutdown." << std::endl;

        // TODO: properly delete remaining entities

        m_entities.clear();
        for(auto entry : m_factories)
        {
            delete entry.second;
        }
        m_factories.clear();

        return true;
    }

    void define_entity(
            const arc::str::UTF8String& id,
            omi::GameEntityCreate* create_func,
            omi::GameEntityDestroy* destroy_func)
    {
        if(m_factories.find(id) != m_factories.end())
        {
            #ifndef OMI_API_MODE_PRODUCTION
                throw arc::ex::KeyError(
                    "Duplicate entities detected with id: \"" + id + "\""
                );
            #else
                return;
            #endif
        }
        assert(create_func != nullptr);
        assert(destroy_func != nullptr);

        EntityFactory* factory = new EntityFactory();
        factory->create_func = create_func;
        factory->destroy_func = destroy_func;

        // add to the mapping
        m_factories.insert(std::make_pair(id, factory));

        m_stat_registered_entity_types.set_at(
            0,
            m_stat_registered_entity_types.at(0) + 1
        );
    }

    void update()
    {
        // stat the number of active entities
        m_stat_active_entities.set_at(0, m_entities.size());

        m_in_update = true;

        for(Entity* entity : m_entities)
        {
            entity->update();
        }

        // continue updating new entities until there are none left
        while(!m_new_entities.empty())
        {
            std::list<Entity*> temp = m_new_entities;
            m_new_entities.clear();
            for(Entity* entity : temp)
            {
                entity->update();
            }

            // move to the back of the current list
            m_entities.insert(m_entities.end(), temp.begin(), temp.end());
        }

        process_removed_components();
        process_new_components();

        // pass in active camera changes
        if(m_camera_changed)
        {
            render::RenderSubsystem::instance().set_active_camera(
                m_active_camera
            );
            m_camera_changed = false;
        }

        m_in_update = false;
    }

    void new_entity(
            const arc::str::UTF8String& id,
            const arc::str::UTF8String& name,
            const omi::Attribute& data)
    {
        // look up id
        auto f_factory = m_factories.find(id);
        if(f_factory == m_factories.end())
        {
            #ifndef OMI_API_MODE_PRODUCTION
                throw arc::ex::KeyError(
                    "No entity with id: \"" + id + "\""
                );
            #else
                return;
            #endif
        }

        // construct the new entity
        Entity* entity = static_cast<Entity*>(
            f_factory->second->create_func(name, data)
        );

        // is an update in progress
        if(!m_in_update)
        {
            m_entities.push_back(entity);
        }
        else
        {
            m_new_entities.push_back(entity);
        }
    }

    const omi::scene::Camera* get_active_camera() const
    {
        return m_active_camera;
    }

    void set_active_camera(const omi::scene::Camera* camera)
    {
        m_active_camera = camera;
        m_camera_changed = true;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // processes components that have been removed from the scene (and removes
    // them from the respective subsystems).
    void process_removed_components()
    {
        for(Entity* entity : m_entities)
        {
            for(AbstractComponent* component :
                entity->retrieve_removed_components())
            {
                switch(component->get_component_type())
                {
                    case ComponentType::kRenderable:
                    {
                        // TODO: make sure this isn't the active camera

                        AbstractRenderable* renderable =
                            static_cast<AbstractRenderable*>(component);
                        render::RenderSubsystem::instance().remove_renderable(
                            renderable
                        );
                        break;
                    }
                    default:
                    {
                        // do nothing
                        break;
                    }
                }
                delete component;
            }
        }
    }

    // processes components that have been newly added to the scene (and adds
    // them to the respective subsystems).
    void process_new_components()
    {
        for(Entity* entity : m_entities)
        {
            for(AbstractComponent* component :
                entity->retrieve_new_components())
            {
                switch(component->get_component_type())
                {
                    case ComponentType::kRenderable:
                    {
                        AbstractRenderable* renderable =
                            static_cast<AbstractRenderable*>(component);
                        render::RenderSubsystem::instance().add_renderable(
                            renderable
                        );
                        break;
                    }
                    default:
                    {
                        // do nothing
                        break;
                    }
                }
            }
        }
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT SceneState& SceneState::instance()
{
    static SceneState inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT bool SceneState::startup_routine()
{
    return m_impl->startup_routine();
}

OMI_API_EXPORT bool SceneState::shutdown_routine()
{
    return m_impl->shutdown_routine();
}

OMI_API_EXPORT void SceneState::define_entity(
            const arc::str::UTF8String& id,
            omi::GameEntityCreate* create_func,
            omi::GameEntityDestroy* destroy_func)
{
    m_impl->define_entity(id, create_func, destroy_func);
}

OMI_API_EXPORT void SceneState::update()
{
    m_impl->update();
}

OMI_API_EXPORT void SceneState::new_entity(
        const arc::str::UTF8String& id,
        const arc::str::UTF8String& name,
        const omi::Attribute& data)
{
    m_impl->new_entity(id, name, data);
}

OMI_API_EXPORT const omi::scene::Camera* SceneState::get_active_camera() const
{
    return m_impl->get_active_camera();
}

OMI_API_EXPORT void SceneState::set_active_camera(
        const omi::scene::Camera* camera)
{
    return m_impl->set_active_camera(camera);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

SceneState::SceneState()
    : m_impl(new SceneStateImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

SceneState::~SceneState()
{
    delete m_impl;
}

} // namespace scene
} // namespace

