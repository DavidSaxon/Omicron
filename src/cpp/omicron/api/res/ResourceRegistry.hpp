/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_RES_RESOURCEREGISTRY_HPP_
#define OMICRON_API_RES_RESOURCEREGISTRY_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/res/ResourceId.hpp"


//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace arc
{
namespace io
{
namespace sys
{

class FileReader;

} // namespace sys
} // namespace io
} // namespace arc

namespace omi
{

class Attribute;

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
    //                              TYPE DEFINITIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Definition of a function that can be used to load resources from
     *        a file stream.
     */
    typedef omi::Attribute (LoaderFunc)(arc::io::sys::FileReader&);

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the ResourceRegistry.
     */
    OMI_API_EXPORT static ResourceRegistry* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Initialises Omicron's ResourceRegistry.
     */
    OMI_API_EXPORT bool startup_routine();

    /*!
     * \brief Shutdowns Omicron's ResourceRegistry.
     */
    OMI_API_EXPORT bool shutdown_routine();

    /*!
     * \brief Adds a new resource loader function to the registry.
     *
     * \param function The function that can be used to load a resource from a
     *                 file stream.
     * \param extension The file extension this function should be used to load.
     */
    OMI_API_EXPORT void define_loader(
            LoaderFunc* function,
            const arc::str::UTF8String& extension);

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether the resource is currently loaded or not.
     */
    OMI_API_EXPORT bool is_loaded(ResourceId id) const;

    // TOOD: in async queue?

    // TODO: DOC once async loading is thing
    // TODO: this resource could be an mutable copy or the true mutable version
    OMI_API_EXPORT omi::Attribute get(ResourceId id);

    // TODO: DOC
    OMI_API_EXPORT void load_blocking(ResourceId id);

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
