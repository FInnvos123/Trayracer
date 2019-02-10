#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>

enum class PrimitiveType
{
    primitive,
    sphere,
    box,

    count
};

struct Primitive
{
    int type; /* 0 = sphere, 1 = box */
    int index;
    int material;
    Primitive(int t, int idx, int matidx) : type(t),
                                            index(idx),
                                            material(matidx)
    {}
};

struct Sphere
{
    glm::vec3 pos;
    float r;
    Sphere(glm::vec3 position, float radius) : pos(position),
                                               r(radius)
    {}
};

struct Box
{
    glm::vec4 min;
    glm::vec4 max;
    Box(glm::vec3 p1, glm::vec3 p2) : min(glm::vec4(p1, 0)),
                                      max(glm::vec4(p2, 0))
    {}
};

#endif /* ifndef PRIMITIVES_H */
