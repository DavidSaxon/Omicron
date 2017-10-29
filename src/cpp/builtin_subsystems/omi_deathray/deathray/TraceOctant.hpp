/*!
 * \file
 * \author David Saxon
 */
#ifndef DEATHRAY_TRACEOCTANT_HPP_
#define DEATHRAY_TRACEOCTANT_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/Matrix.hpp>
#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include <deathray/gl/ShaderProgram.hpp>


namespace death
{

// TODO: DOC
class TraceOctant
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    ARC_LX_ALIGNED_NEW

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    TraceOctant(const arc::lx::Vector3f& position, float size);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~TraceOctant();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    const arc::lx::Vector3f& get_position() const;

    void draw_gl_debug(const arc::lx::Matrix44f& vp_matrix);

private:

    //--------------------------------------------------------------------------
    //                               PRIVATE STRUCT
    //--------------------------------------------------------------------------

    union OctantId
    {
        arc::uint8 bytes[3];
        arc::uint32 integer;
    };

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    arc::lx::Vector3f m_position;
    float m_size;

    OctantId m_id;

    // gl data
    death::gl::ShaderProgram m_gl_program;
    GLuint m_gl_vao;
    std::size_t m_gl_point_count;
    arc::lx::Vector3f m_gl_colour;
};

} // namespace death

#endif
