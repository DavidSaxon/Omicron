/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_RES_RESOURCEREGISTRY_HPP_
#define OMICRON_API_RES_RESOURCEREGISTRY_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/res/ResourceId.hpp"

namespace omi
{
namespace res
{

/*!
 * \brief Singleton object that discovers the locations of avialable resources
 *        and provides functionality for loading resources either blocking or
 *        asynchronously.
 */
class ResourceRegistry
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the ResourceRegistry.
     */
    OMI_API_GLOBAL static ResourceRegistry* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Initialises Omicron's ResourceRegistry.
     */
    OMI_API_GLOBAL bool startup_routine();

    /*!
     * \brief Shutdowns Omicron's ResourceRegistry.
     */
    OMI_API_GLOBAL bool shutdown_routine();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    // TODO: load blocking from id
    // TODO: load async from id

    // TODO: loading blocking pack
    // TODO: loading async from pack

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    ResourceRegistry();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~ResourceRegistry();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class ResourceRegistryImpl;
    ResourceRegistryImpl* m_impl;
};

} // namespace res
} // namespace omi

#endif
