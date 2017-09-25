/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_ENGINE_HPP_
#define OMICRON_RUNTIME_ENGINE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>


//------------------------------------------------------------------------------
//                   ____  __  _________________  ____  _   __
//                  / __ \/  |/  /  _/ ____/ __ \/ __ \/ | / /
//                 / / / / /|_/ // // /   / /_/ / / / /  |/ /
//                / /_/ / /  / // // /___/ _, _/ /_/ / /|  /
//                \____/_/  /_/___/\____/_/ |_|\____/_/ |_/
//------------------------------------------------------------------------------


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
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron engine.
     */
    static Engine* instance();

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

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Engine();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class EngineImpl;
    EngineImpl* m_impl;

    friend class EngineImpl;
};

} // namespace runtime
} // namespace omi

#endif
