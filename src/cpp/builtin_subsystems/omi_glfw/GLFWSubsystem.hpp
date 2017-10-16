/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_GLFW_SUBSYSTEM_HPP_
#define OMICRON_GLFW_SUBSYSTEM_HPP_

#include <omicron/api/context/ContextSubsystem.hpp>


namespace omi_glfw
{

/*!
 * \brief The GLFW implementation of Omicron's context subsystem.
 */
class GLFWSubsystem
    : public omi::context::ContextSubsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    GLFWSubsystem();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~GLFWSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual bool startup_routine() override;

    virtual bool shutdown_routine() override;

    virtual void main_loop(EngineCycleFunc* engine_cycle_func) override;
};

} // namespace omi_glfw

#endif
