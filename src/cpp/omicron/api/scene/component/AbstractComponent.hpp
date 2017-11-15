/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_
#define OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_

#include <arcanecore/base/Types.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


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
    kTransform,
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
    OMI_API_EXPORT AbstractComponent();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~AbstractComponent();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the unique id of this component.
     */
    OMI_API_EXPORT ComponentId get_id() const;

    /*!
     * \brief Returns the type of this component.
     */
    virtual ComponentType get_component_type() const = 0;

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Alters this component that another component has become dependent
     *        on it.
     */
    OMI_API_EXPORT void dependent_added(AbstractComponent* dependent) const;

    /*!
     * \brief Alerts this component that a dependent component is no longer
     *        dependent on it.
     */
    OMI_API_EXPORT void dependent_removed(AbstractComponent* dependent) const;

    /*!
     * \brief Alerts this component that another component it is dependent has
     *        been detached from this component.
     *
     * \note Does nothing by default but is implemented by component types that
     *       implement dependencies.
     */
    virtual void dependency_removed(AbstractComponent* component)
    {
    }

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class AbstractComponentImpl;
    AbstractComponentImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
