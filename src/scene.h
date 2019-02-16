#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <vector>
#include <map>

#include "primitives.h"

class Scene
{
    public:
        Scene();

        void addSphere(Sphere* sphere, Material* mat);
        void addPolygon(std::vector<GLfloat>& verts, std::vector<GLfloat>& normals, Material* mat);

        void addPrimitive(PrimitiveType type, void* primitive);
        size_t getPrimitiveCount(PrimitiveType type);
        void* getPrimitive(PrimitiveType type, int index);
    private:
        std::map<PrimitiveType, std::vector<void*>> m_primitives;
};

#endif /* ifndef SCENE_H */
