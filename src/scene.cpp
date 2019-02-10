#include "scene.h"

Scene::Scene()
{
    for (int i = 0; i < (int)PrimitiveType::count; i++) {
        m_primitives[(PrimitiveType)i] = std::vector<void*>();
    }
}

void Scene::addPrimitive(PrimitiveType type, void* primitive)
{
    m_primitives[type].push_back(primitive);
}

size_t Scene::getPrimitiveCount(PrimitiveType type)
{
    return m_primitives[type].size();
}

void* Scene::getPrimitive(PrimitiveType type, int index)
{
    return m_primitives[type].at(index);
}
