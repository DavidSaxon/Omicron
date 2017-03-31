/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_ENGINE_HPP_
#define OMICRON_RUNTIME_ENGINE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

namespace omi
{
namespace runtime
{

/*!
 * \brief Singleton object that owns and starts the Omicron runtime.
 */
class Engine
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Engine();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron engine.
     */
    static Engine* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Begins execution of Omicron.
     *
     *  Control is only returned from this function once Omicron exits.
     */
    int execute();

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    Engine();

};

} // namespace runtime
} // namespace omi

#endif
