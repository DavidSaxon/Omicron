#ifndef OMICRON_PROTO_SCENE_HPP_
#define OMICRON_PROTO_SCENE_HPP_

#include <vector>

#include "omicron/proto/Entity.hpp"

namespace omi
{

class Scene
{
public:

    Scene();
    virtual ~Scene();

    virtual void update() = 0;

protected:

    std::vector<Entity*> m_entities;
};

} // namespace omi

#endif
