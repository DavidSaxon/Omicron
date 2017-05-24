#include "pxtrace/PXSubsystem.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/clock/ClockOperations.hpp>
#include <arcanecore/gm/VectorMath.hpp>

#include <omicron/api/report/Logging.hpp>

#include "pxtrace/PXGlobals.hpp"


//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(PXSubsystem);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

PXSubsystem::PXSubsystem()
    : m_frame_buffer(nullptr)
    , m_camera(nullptr)
{
    // TODO: setup logger
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

PXSubsystem::~PXSubsystem()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void PXSubsystem::startup()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("PXTRACE"));

    global::logger->info << "Starting up subsystem." << std::endl;

    // create the camera
    m_camera.reset(new Camera(1.0F));

    // add a sphere
    m_spheres.emplace_back(
        new Sphere(arc::gm::SimdVector3f(0.0F, 0.0F, 8.0F), 1.0F)
    );
}

void PXSubsystem::shutdown()
{
    global::logger->info << "Shutting down subsystem." << std::endl;

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);
}

void PXSubsystem::setup_rendering()
{
    global::logger->debug << "Setting up rendering context" << std::endl;

    // TODO: need to be able to get the resolution

    // TODO:
    glewExperimental = GL_TRUE;
    glewInit();

    // get the OpenGL version
    GLint gl_major_version = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &gl_major_version);
    GLint gl_minor_version = 0;
    glGetIntegerv(GL_MINOR_VERSION, &gl_minor_version);

    global::logger->info
        << "Detected OpenGL version: " << gl_major_version << "."
        << gl_minor_version << std::endl;

    // check the version is 3.0+
    if(gl_major_version < 3)
    {
        arc::str::UTF8String error_message;
        error_message
            << "pxtrace requires an OpenGL context of 3.0 or greater. OpenGL "
            << "version " << gl_major_version << "." << gl_minor_version
            << " detected.";
        throw arc::ex::ContextError(error_message);
    }

    // create the frame buffer
    m_frame_buffer.reset(new FrameBuffer(arc::gm::Vector2u(1920, 1080)));
}

// TODO: shutdown rendering

void PXSubsystem::render()
{
    arc::uint64 render_time = arc::clock::get_current_time();

    // clear the frame buffer before starting
    m_frame_buffer->clear(arc::gm::Vector3f(0.7F, 0.7F, 0.7F));

    // fire out the rays
    const arc::gm::Vector2u& resolution = m_frame_buffer->get_dimensions();
    float aspect_ratio =
        static_cast<float>(resolution.x()) / static_cast<float>(resolution.y());
    arc::gm::SimdVector3f ray_origin = m_camera->get_focal_point();
    for(std::size_t y = 0; y < resolution.y(); ++y)
    {
        for(std::size_t x = 0; x < resolution.x(); ++x)
        {
            arc::gm::SimdVector3f ray_direction(
                (((static_cast<float>(x) / static_cast<float>(resolution.x()))
                * 0.2F) - 0.1F) * aspect_ratio,
                (((static_cast<float>(y) / static_cast<float>(resolution.y()))
                * 0.2F) - 0.1F),
                -ray_origin.z()
            );
            ray_direction = arc::gm::normalise(ray_direction);

            // check if the ray hits a sphere
            for(const std::unique_ptr<Sphere>& sphere : m_spheres)
            {
                float t0 = 0.0F;
                float t1 = 0.0F;
                if(sphere->intersects(ray_origin, ray_direction, t0, t1))
                {
                    m_frame_buffer->set(
                        arc::gm::Vector2u(x, y),
                        arc::gm::Vector3f(0.0F, 0.0F, 1.0F)
                    );
                    break;
                }
            }
        }
    }

    m_frame_buffer->render();

    render_time = arc::clock::get_current_time() - render_time;

    global::logger->notice
        << "pxtrace frame took " << render_time << "ms :: "
        << static_cast<arc::uint64>(1000.0F / static_cast<float>(render_time))
        << "fps" << std::endl;
}
