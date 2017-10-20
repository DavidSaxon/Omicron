#include "omicron/api/scene/SceneState.hpp"

#include <list>
#include <memory>
#include <unordered_set>

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/api/scene/Entity.hpp"


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

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the entities current within the scene
    std::list<std::unique_ptr<Entity>> m_entities;

    #ifndef OMI_API_MODE_PRODUCTION

        // set that is used to quickly check for duplicate entities
        std::unordered_set<Entity*> m_duplicate_check;

    #endif

    // whether an update cycle is in progress or not
    bool m_in_update;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SceneStateImpl()
        : m_in_update(false)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~SceneStateImpl()
    {
        m_entities.clear();
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void update()
    {
        m_in_update = true;

        for(std::unique_ptr<Entity>& entity : m_entities)
        {
            entity->update();
        }

        m_in_update = false;
    }

    void add_entity(Entity* entity)
    {
        // TODO: need to support entities being added during the update cycle.

        // check for duplicates
        #ifndef OMI_API_MODE_PRODUCTION

            if(m_duplicate_check.find(entity) != m_duplicate_check.end())
            {
                throw arc::ex::ValueError(
                    "Attempted to add duplicate entity \"" +
                    entity->get_name() + "\" to SceneState"
                );
            }

            m_duplicate_check.insert(entity);

        #endif

        m_entities.emplace_back(entity);
    }

    // TODO: state the current number of entities in update
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

OMI_API_EXPORT void SceneState::update()
{
    m_impl->update();
}

OMI_API_EXPORT void SceneState::add_entity(Entity* entity)
{
    m_impl->add_entity(entity);
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

