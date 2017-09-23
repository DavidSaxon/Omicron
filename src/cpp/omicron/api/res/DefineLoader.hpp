/*!
 * \file
 * \author David Saxon
 * \brief Macros for define resource loader functions.
 */
#ifndef OMICRON_API_RES_DEFINELOADER_HPP_
#define OMICRON_API_RES_DEFINELOADER_HPP_

#include <omicron/api/res/ResourceRegistry.hpp>

// TODO: DOC
#define OMICRON_API_RES_DEFINE_LOADER(function, extension)                     \
    namespace                                                                  \
    {                                                                          \
    class StaticDefineLoader                                                   \
    {                                                                          \
    public:                                                                    \
        StaticDefineLoader()                                                   \
        {                                                                      \
            omi::res::ResourceRegistry::instance()->define_loader(             \
                function,                                                      \
                extension                                                      \
            );                                                                 \
        }                                                                      \
    };                                                                         \
    static const StaticDefineLoader define_loader;                             \
    }

#endif
