/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_GL_BOOTSTRAP_HPP_
#define OMI_GL_BOOTSTRAP_HPP_

#include <omicron/api/render/subsystem/RenderBootstrap.hpp>


namespace omi_gl
{

/*!
 * \brief Handles startup and shutdown of the Omicron GL rendering subsystem.
 */
class GLBootstrap : public omi::render::ss::Bootstrap
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~GLBootstrap();

    //--------------------------------------------------------------------------
    //                           PUBLIC STATIC FUNTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron GL bootstrapper.
     */
    static GLBootstrap* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();

    // override
    virtual void shutdown();

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTORS
    //--------------------------------------------------------------------------

    GLBootstrap();

};

} // namespace omi_gl

#endif
