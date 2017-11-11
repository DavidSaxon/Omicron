#include "omi_deathray/DeathSubsystem.hpp"

#include <omicron/api/report/Logging.hpp>
#include <omicron/api/scene/component/renderable/AbstractRenderable.hpp>

#include <deathray/Renderer.hpp>

#include "omi_deathray/DeathGlobals.hpp"


namespace omi_death
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DeathSubsystem::DeathSubsystem()
    : omi::render::RenderSubsystem()
    // TODO: REMOVE ME
    , m_projection_matrix(arc::lx::Matrix44f::Identity())
    , m_view_matrix      (arc::lx::Matrix44f::Identity())
    , m_rotation         (0.0F * 0.0174533F, 0.0F, 0.0F)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DeathSubsystem::~DeathSubsystem()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool DeathSubsystem::startup_routine()
{
    // set up logging
    global::logger = omi::report::log_handler.vend_input(
        arc::log::Profile("OMICRON-DEATHRAY")
    );

    global::logger->debug
        << "Starting Omicron DeathRay render subsystem." << std::endl;

    return true;
}

bool DeathSubsystem::firstframe_routine()
{
    global::logger->debug
        << "First-frame setup of Omicron DeathRay render subsystem."
        << std::endl;

    // TODO; why crash??
    // generate the death ray scene
    death_scene_gen(&m_scene);
    death_scene_set_resolution(m_scene, 1000, 1000);

    // generate the camera
    death_cam_gen(1, &m_camera);
    // TODO:
    death_cam_set_properties(
        m_camera,
        2.0F,                // focal length
        0.0F,                // filmback origin x
        0.0F,                // filmback origin y
        1.0F,                // filmback width
        1.0F                 // filmback height
    );
    // attach the camera to the scene
    death_scene_set_camera(m_scene, m_camera);

    // TODO: REMOVE BELOW
    //--------------------------------------------------------------------------

    // TODO: ensure this can safely done twice
    // initialise GLEW
    GLenum glew_error = glewInit();
    if(glew_error != GLEW_OK)
    {
        global::logger->critical
            << "Error initialising GLEW: " << glewGetErrorString(glew_error)
            << std::endl;
    }

    // TODO: check OpenGL version (or do inside DeathRay)

    // TODO: TESTING
    // glClearColor(0.7F, 0.7F, 0.7F, 1.0F);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    // glCullFace(GL_BACK);
    // glEnable(GL_DEPTH_TEST);

    // // TODO: get from context (needs to handle updates - could be called once
    // //       per frame)
    // glViewport(0, 0, 350, 350);

    // TODO: TESTING
    float fov = 90.0F;
    float aspect = 350.0F / 350.0F;
    float z_near  = 0.0001F;
    float z_far   = 100000.0F;
    m_projection_matrix = arc::lx::perspective_44f(fov, aspect, z_near, z_far);

    // set up renderer
    // death::Renderer::instance().setup();

    return true;
}

bool DeathSubsystem::shutdown_routine()
{
    global::logger->debug
        << "Shutting down Omicron DeathRay render subsystem."
        << std::endl;

    death_cam_delete(1, &m_camera);
    death_scene_delete(&m_scene);

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);

    return true;
}

void DeathSubsystem::add_renderable(
        omi::scene::AbstractRenderable* renderable)
{
    switch(renderable->get_renderable_type())
    {
        case omi::scene::RenderableType::kMesh:
        {
            omi::scene::Mesh* component =
                static_cast<omi::scene::Mesh*>(renderable);
            m_meshes.push_back(new DeathMesh(component, m_scene));
            break;
        }
        default:
        {
            global::logger->warning
                << "Unknown renderable type: "
                << static_cast<int>(renderable->get_renderable_type())
                << " passed to DeathRay render subsystem." << std::endl;
            break;
        }
    }
}

void DeathSubsystem::remove_renderable(
        omi::scene::AbstractRenderable* renderable)
{
    // TODO: REMOVE ME
    global::logger->notice
        << "renderable remove" << std::endl;
}

void DeathSubsystem::render()
{
    // TODO:
    m_rotation(1) += 0.25F * 0.0174533F;
    // TODO: testing
    m_view_matrix = arc::lx::Matrix44f::Identity();
    m_view_matrix *= arc::lx::rotate_euler_44f(m_rotation);
    m_view_matrix *=
        // arc::lx::translate_44f(arc::lx::Vector3f(0.0F, 1.0F, 2.0F));
        arc::lx::translate_44f(arc::lx::Vector3f(0.0F, 0.0F, 4.0F));
    // m_view_matrix *= arc::lx::rotate_euler_44f(
        // arc::lx::Vector3f(-25.0F * 0.0174533F, 0.0F, 0.0F));


    death_cam_set_transform(m_camera, &m_view_matrix(0, 0));
    death_scene_render(m_scene);

    // // TODO: REMOVE BELOW HERE
    //--------------------------------------------------------------------------

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // // TODO:
    // m_rotation(1) += 0.25F * 0.0174533F;
    // // TODO: testing
    // m_view_matrix = arc::lx::Matrix44f::Identity();
    // m_view_matrix *=
    //     arc::lx::translate_44f(arc::lx::Vector3f(0.0F, -0.085F, -0.25F));
    // m_view_matrix *= arc::lx::rotate_euler_44f(m_rotation);

    // death::Renderer::instance().render();

    // TODO: revert
    // for(DeathMesh* mesh : m_meshes)
    // {
    //     mesh->render(m_projection_matrix * m_view_matrix);
    // }
}

} // namespace omi_death

OMI_RENDER_REGISTER_SUBSYSTEM("0.0.1", omi_death::DeathSubsystem);
