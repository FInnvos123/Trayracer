#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>

#include "primitives.h"

class Scene
{
    public:
        Scene();

        void addPrimitive(PrimitiveType type, void* primitive);
        size_t getPrimitiveCount(PrimitiveType type);
        void* getPrimitive(PrimitiveType type, int index);
    private:
        std::map<PrimitiveType, std::vector<void*>> m_primitives;
};

#endif /* ifndef SCENE_H */
