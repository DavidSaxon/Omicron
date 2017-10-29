/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_
#define OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_

#include <arcanecore/base/Types.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                     ENUMS
//------------------------------------------------------------------------------

/*!
 * \brief Defines the different directly derived component types.
 */
enum class ComponentType
{
    kTrivial,
    kRenderable,
    // TODO: more types
};

//------------------------------------------------------------------------------
//                                TYPE DEFINITIONS
//------------------------------------------------------------------------------

/*!
 * \brief Represents a unique id assigned to each component in the engine.
 */
typedef arc::uint64 ComponentId;

/*!
 * \brief A component of a game entity - components are managed by the engine
 *        and will be passed to the correct subsystem at runtime.
 */
class AbstractComponent
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor.
     */
    AbstractComponent()
        : m_id(0)
    {
        static ComponentId g_compontent = 0;
        ++g_compontent;
        m_id = g_compontent;
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractComponent()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the unique id of this component.
     */
    ComponentId get_id() const
    {
        return m_id;
    }

    /*!
     * \brief Returns the type of this component.
     */
    virtual ComponentType get_component_type() const = 0;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    ComponentId m_id;
};

} // namespace scene
} // namespace omi

#endif
