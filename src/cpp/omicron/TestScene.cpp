#include "omicron/TestScene.hpp"

#include "omicron/TestEntity.hpp"

TestScene::TestScene()
{
    m_entities.push_back(new TestEntity());
}

TestScene::~TestScene()
{
}

void TestScene::update()
{
    std::vector<omi::Entity*>::iterator entity;
    for(entity = m_entities.begin(); entity != m_entities.end(); ++entity)
    {
        (*entity)->update();
    }
}
