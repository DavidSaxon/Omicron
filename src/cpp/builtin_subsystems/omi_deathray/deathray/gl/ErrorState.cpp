#include "deathray/gl/ErrorState.hpp"

#include <iostream>


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
    // TODO: better place to log to
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Encountered GL error";
        if(!action.is_empty())
        {
            std::cout << "while " << action;
        }
        std::cout << ": " << errorcode_to_string(err) << std::endl;
    }
}

} // namespace error
} // namespace gl
} // namespace death
