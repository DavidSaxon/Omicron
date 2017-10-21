#include "omicron/api/scene/SceneState.hpp"

#include <cassert>
#include <list>
#include <unordered_map>
#include <unordered_set>

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/api/report/Logging.hpp"
#include "omicron/api/report/stats/StatsDatabase.hpp"
#include "omicron/api/scene/Entity.hpp"
#include "omicron/api/scene/SceneGlobals.hpp"


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

    // stats
    omi::Int64Attribute m_stat_registered_entity_types;
    omi::Int64Attribute m_sta_active_entities;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SceneStateImpl()
        : m_in_update                   (false)
        , m_stat_registered_entity_types(0, false)
        , m_sta_active_entities         (0, false)
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
            m_sta_active_entities,
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
        m_sta_active_entities.set_at(0, m_entities.size());

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

