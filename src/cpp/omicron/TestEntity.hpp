#ifndef OMICRON_TESTENTITY_HPP_
#define OMICRON_TESTENTITY_HPP_

#include "omicron/proto/Entity.hpp"

class TestEntity : public omi::Entity
{
public:

    TestEntity();
    virtual ~TestEntity();

    virtual void update();
};

#endif
