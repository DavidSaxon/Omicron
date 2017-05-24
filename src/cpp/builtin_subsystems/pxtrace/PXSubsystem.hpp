/*!
 * \file
 * \author David Saxon
 */
#ifndef PXTRACE_SUBSYSTEM_HPP_
#define PXTRACE_SUBSYSTEM_HPP_

#include <memory>
#include <vector>

#include <omicron/subsystem/Renderer.hpp>

#include "pxtrace/Camera.hpp"
#include "pxtrace/FrameBuffer.hpp"
#include "pxtrace/Sphere.hpp"


/*!
 * \brief TODO
 */
class PXSubsystem : public omi::ss::Renderer
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    PXSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    virtual ~PXSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();

    // override
    virtual void shutdown();

    // override
    virtual void setup_rendering();

    // override
    virtual void render();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    std::unique_ptr<FrameBuffer> m_frame_buffer;
    std::unique_ptr<Camera> m_camera;
    std::vector<std::unique_ptr<Sphere>> m_spheres;
};

#endif
