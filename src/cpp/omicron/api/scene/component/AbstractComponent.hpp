/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_
#define OMICRON_API_SCENE_COMPONENT_ABSTRACTCOMPONENT_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>


namespace omi
{
namespace scene
{

// TODO: DOC
class AbstractComponent
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    // TODO:
};

} // namespace scene
} // namespace

#endif
