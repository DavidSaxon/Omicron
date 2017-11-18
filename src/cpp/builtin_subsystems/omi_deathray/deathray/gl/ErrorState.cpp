#include "deathray/gl/ErrorState.hpp"

#include <iostream>

#include "deathray/impl/Globals.hpp"


namespace death
{
namespace gl
{
namespace error
{

DEATH_API_EXPORT arc::str::UTF8String errorcode_to_string(GLenum error_code)
{
    switch(error_code)
    {
        case GL_INVALID_ENUM:
        {
            return "GL_INVALID_ENUM";
        }
        case GL_INVALID_VALUE:
        {
            return "GL_INVALID_VALUE";
        }
        case GL_INVALID_OPERATION:
        {
            return "GL_INVALID_OPERATION";
        }
        case GL_STACK_OVERFLOW:
        {
            return "GL_STACK_OVERFLOW";
        }
        case GL_STACK_UNDERFLOW:
        {
            return "GL_STACK_UNDERFLOW";
        }
        case GL_OUT_OF_MEMORY:
        {
            return "GL_OUT_OF_MEMORY";
        }
        case GL_INVALID_FRAMEBUFFER_OPERATION:
        {
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        }
        case GL_CONTEXT_LOST:
        {
            return "GL_CONTEXT_LOST";
        }
        case GL_TABLE_TOO_LARGE:
        {
            return "GL_TABLE_TOO_LARGE";
        }
        default:
        {
            return "UNKOWN";
        }
    }
}

DEATH_API_EXPORT void check_state(const arc::str::UTF8String& action)
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        DEATH_LOG_ERROR(
            "Encountered GL error " << action << ": "
            << errorcode_to_string(err)
        );
    }
}

} // namespace error
} // namespace gl
} // namespace death
