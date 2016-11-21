#ifndef OMICRON_TESTSCENE_HPP_
#define OMICRON_TESTSCENE_HPP_

#include "omicron/proto/Scene.hpp"

class TestScene : public omi::Scene
{
public:

    TestScene();
    virtual ~TestScene();

    virtual void update();
};

#endif
