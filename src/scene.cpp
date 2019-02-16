#include "scene.h"

Scene::Scene()
{
    for (int i = 0; i < (int)PrimitiveType::count; i++) {
        m_primitives[(PrimitiveType)i] = std::vector<void*>();
    }
}

void Scene::addSphere(Sphere* sphere, Material* mat)
{
    addPrimitive(PrimitiveType::primitive,
                 new Primitive(0,
                               getPrimitiveCount(PrimitiveType::sphere),
                               getPrimitiveCount(PrimitiveType::material)));
    addPrimitive(PrimitiveType::sphere, sphere);
    addPrimitive(PrimitiveType::material, mat);
}

void Scene::addPolygon(std::vector<GLfloat>& verts, std::vector<GLfloat>& normals, Material* mat)
{
    for (int i = 0, j = 0; i < verts.size(); i+=9, j+=3) {
        addPrimitive(PrimitiveType::primitive,
                     new Primitive(1,
                                   getPrimitiveCount(PrimitiveType::triangle),
                                   getPrimitiveCount(PrimitiveType::material)));
        glm::vec3 v0 = glm::vec3(verts[i], verts[i+1], verts[i+2]);
        glm::vec3 v1 = glm::vec3(verts[i+3], verts[i+4], verts[i+5]);
        glm::vec3 v2 = glm::vec3(verts[i+6], verts[i+7], verts[i+8]);
        glm::vec3 n = glm::vec3(normals[j], normals[j+1], normals[j+2]);

        addPrimitive(PrimitiveType::triangle, new Triangle(v0, v1, v2, n));
    }

    addPrimitive(PrimitiveType::material, mat);
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
