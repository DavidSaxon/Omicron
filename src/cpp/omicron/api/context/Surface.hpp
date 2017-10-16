/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_CONTEXT_SURFACE_HPP_
#define OMICRON_API_CONTEXT_SURFACE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"


namespace omi
{
namespace context
{

/*!
 * \brief Singleton which can be used to query and control the rendering surface
 *        of Omicron.
 */
class Surface
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron surface.
     */
    OMI_API_EXPORT static Surface* instance();

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Sets the implementation that will be used by the current runtime.
     */
    OMI_API_EXPORT static void set_implementation(
            omi::SubsytemObject<Surface>* impl);

    /*!
     * \brief Destroys the current implementation object.
     */
    OMI_API_EXPORT static void destroy();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: get native resolution

    // TODO: get position

    // TODO: get context info

    // TODO: is_resizable

    // TODO: is_moveable

    // TODO: set_size

    // TODO: set_position

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT Surface();

    //--------------------------------------------------------------------------
    //                            PROTECTED DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Surface();
};

} // namespace context
} // namespace omi

#endif
