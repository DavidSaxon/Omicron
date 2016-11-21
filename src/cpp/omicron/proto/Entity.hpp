#ifndef OMICRON_PROTO_ENTITY_HPP_
#define OMICRON_PROTO_ENTITY_HPP_

namespace omi
{

class Entity
{
public:

    Entity();
    virtual ~Entity();

    virtual void update() = 0;
};

} // namespace omi

#endif
