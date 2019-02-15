#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>

enum class PrimitiveType
{
    material,
    pointlight,
    primitive,
    sphere,
    triangle,

    count
};

struct Material
{
    glm::vec4 col_diffuse;
    glm::vec4 col_specular;
    float spec;
    float refl;
    float ref_index;
    float pad;
    Material(glm::vec4 c_diff, glm::vec4 c_spec,
             float s = 0, float r = 0, float ri = 0)
        : col_diffuse(c_diff),
          col_specular(c_spec),
          spec(s),
          refl(r),
          ref_index(ri)
    {}
};

struct Pointlight
{
    glm::vec4 pos;
    glm::vec4 col;
    Pointlight(glm::vec3 position, glm::vec4 color) : pos(glm::vec4(position, 1)),
                                                      col(color)
    {}
};

struct Primitive
{
    int type; /* 0 = sphere, 1 = triangle */
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

struct Triangle
{
    glm::vec3 v0;
    float p1;
    glm::vec3 v1;
    float p2;
    glm::vec3 v2;
    float p3;
    glm::vec3 n;
    float p4;
    Triangle(glm::vec3 vert0, glm::vec3 vert1, glm::vec3 vert2) : v0(vert0),
                                                                  v1(vert1),
                                                                  v2(vert2)
    {
        n = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    }
};

#endif /* ifndef PRIMITIVES_H */
