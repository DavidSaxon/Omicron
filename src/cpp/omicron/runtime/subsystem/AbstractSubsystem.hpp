/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_ABSTRACTSUBSYSTEM_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_ABSTRACTSUBSYSTEM_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>


namespace omi
{
namespace runtime
{
namespace ss
{

/*!
 * \brief Abstract base class that defines an object which manages the
 *        components used to access an Omicron subsystem.
 */
class AbstractSubsystem
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    AbstractSubsystem()
    {
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~AbstractSubsystem()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Loads the subsystem from the given dynamic library and binds it
     *        into Omicron.
     */
    virtual void bind(arc::io::dl::Handle library) = 0;

    /*!
     * \brief Starts up this subsystem.
     */
    virtual void startup() = 0;

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
