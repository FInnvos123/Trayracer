#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>

enum class PrimitiveType
{
    primitive,
    sphere,
    plane,

    count
};

struct Primitive
{
    int type; /* 0 = sphere */
    int index;
    int material;
    Primitive(int t, int idx, int matidx) : type(t), index(idx), material(matidx) {}
};

struct Sphere
{
    glm::vec3 pos;
    float r;
    Sphere(glm::vec3 position, float radius) : pos(position), r(radius) {}
};

struct Plane
{
    glm::vec3 norm;
    float d;
    Plane(glm::vec3 normal, float dist) : norm(normal), d(dist) {}
};

#endif /* ifndef SPHERE_H */
