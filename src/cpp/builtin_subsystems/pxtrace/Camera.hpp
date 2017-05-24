/*!
 * \file
 * \author David Saxon
 */
#ifndef PXTRACE_CAMERA_HPP_
#define PXTRACE_CAMERA_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/memory/Alignment.hpp>
#include <arcanecore/gm/Vector.hpp>


class Camera
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
    , public arc::memory::AlignedBase<Camera>
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    Camera(float focal_length);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Camera();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    float get_focal_length() const;

    const arc::gm::SimdVector3f& get_focal_point() const;

    void set_focal_length(float focal_length);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    float m_focal_length;
    arc::gm::SimdVector3f m_focal_point;
};

#endif
