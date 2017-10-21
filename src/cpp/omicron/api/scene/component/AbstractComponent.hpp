/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_
#define OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_

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
    {
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
     * \brief Returns the type of this component.
     */
    virtual ComponentType get_component_type() const = 0;
};

} // namespace scene
} // namespace omi

#endif
