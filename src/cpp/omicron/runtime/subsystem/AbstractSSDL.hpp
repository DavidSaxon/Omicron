/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_ABSTRACTSSDL_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_ABSTRACTSSDL_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>


namespace omi
{
namespace runtime
{
namespace ss
{

/*!
 * \brief Abstract base class that defines an object that provides access to
 *        a subsystem contained within a dynamic library.
 */
class AbstractSSDL
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    AbstractSSDL()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractSSDL()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Loads the subsystem from the given library path and binds it into
     *        Omicron.
     *
     * \return Whether a subsystem of the implemented type could be bound from
     *         the given library.
     */
    virtual bool bind(const arc::io::sys::Path& path) = 0;

    /*!
     * \brief Unbinds the subsystem from Omicron and shuts it down.
     *
     * \warning This should close the dynamic library, this is done by the
     *          SubsystemManager.
     */
    virtual void release() = 0;
};

} // namespace ss
} // namespace runtime
} // namespace omi

#endif
