#include "pxtrace/Camera.hpp"


//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Camera::Camera(float focal_length)
    : m_focal_length(focal_length)
    , m_focal_point (0.0F, 0.0F, -focal_length)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Camera::~Camera()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

float Camera::get_focal_length() const
{
    return m_focal_length;
}

const arc::gm::SimdVector3f& Camera::get_focal_point() const
{
    return m_focal_point;
}

void Camera::set_focal_length(float focal_length)
{
    m_focal_length = focal_length;
    m_focal_point = arc::gm::SimdVector3f(0.0F, 0.0F, -focal_length);
}
