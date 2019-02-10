#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>

enum class PrimitiveType
{
    material,
    pointlight,
    primitive,
    sphere,
    box,

    count
};

struct Material
{
    glm::vec4 col_diffuse;
    glm::vec4 col_specular;
    float spec;
    float refl;
    int dielectric;
    float ref_index;
    Material(glm::vec4 c_diff, glm::vec4 c_spec,
             float s = 0, float r = 0,
             int d = 0, float ri = 0)
        : col_diffuse(c_diff),
          col_specular(c_spec),
          spec(s),
          refl(r),
          dielectric(d),
          ref_index(ri)
    {}
};

struct Pointlight
{
    glm::vec4 pos;
    glm::vec4 col;
    Pointlight(glm::vec3 position, glm::vec4 color) : pos(glm::vec4(position, 0)),
                                                      col(color)
    {}
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
