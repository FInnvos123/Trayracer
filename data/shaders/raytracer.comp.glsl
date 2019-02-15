#version 450

#define FAR_CLIP 100.0f
#define EPSILON 0.0001f

#define SPHERE 0
#define TRIANGLE 1

#define MAX_REFLECTIONS 8

layout(local_size_x = 16, local_size_y = 16) in;
layout(rgba32f, binding = 0) uniform writeonly image2D dest_tex;

/* Uniforms */

uniform vec3 cam_pos;
uniform vec3 p0;
uniform vec3 p0p1;
uniform vec3 p0p2;

uniform int screen_width;
uniform int screen_height;

/* Structs */

struct Ray
{
    vec3 origin;
    vec3 dir;
};

struct Material
{
    vec4 col_diffuse;
    vec4 col_specular;
    float spec;
    float refl;
    float ref_index;
};

struct Pointlight
{
    vec3 pos;
    vec4 color;
};

struct Primitive
{
    int type;
    int index;
    int material;
};

struct Sphere
{
    vec3 pos;
    float r;
};

struct Triangle
{
    vec3 v0;
    vec3 v1;
    vec3 v2;
    vec3 n;
};

/* Object buffers */

uniform int primitiveCount;
uniform int pointlightCount;

layout(std430, binding = 1) buffer materialBuffer { Material materials[]; };
layout(std430, binding = 2) buffer pointlightsBuffer { Pointlight pointlights[]; };
layout(std430, binding = 3) buffer primitiveBuffer { Primitive primitives[]; };
layout(std430, binding = 4) buffer sphereBuffer { Sphere spheres[]; };
layout(std430, binding = 5) buffer trisBuffer { Triangle tris[]; };

float intersectSphere(Ray r, Sphere s)
{
    vec3 oc = r.origin - s.pos;
    float p = -dot(r.dir, oc);
    float d = p * p - dot(oc, oc) + s.r * s.r;
    if (d < 0) return FAR_CLIP;
    else if (d == 0) return (p < 0) ? FAR_CLIP : p;
    else {
        float t1 = sqrt(d);
        float t2 = p + t1;
        t1 = p - t1;

        if (t1 < -EPSILON || t2 < -EPSILON) return FAR_CLIP;
        if (min(t1, t2) < EPSILON) return FAR_CLIP;
        return min(t1, t2);
    }
}

float intersectTriangle(Ray r, Triangle t)
{
    /* Möller-Trumbore intersection algorithm */

    vec3 e1 = t.v1 - t.v0;
    vec3 e2 = t.v2 - t.v0;
    vec3 T = r.origin - t.v0;
    vec3 p = cross(r.dir, e2);
    float f = dot(p, e1);

    float u = dot(p, T) / f;
    if (u < -EPSILON || u > 1.0f)
        return FAR_CLIP;

    vec3 q = cross(T, e1);
    float v = dot(q, r.dir) / f;
    if (v < -EPSILON || u+v > 1.0f)
        return FAR_CLIP;

    return dot(q, e2) / f;
}

vec3 calcNormal(int prim, vec3 intersection)
{
    vec3 n;
    int obj = primitives[prim].index;
    switch (primitives[prim].type) {
        case SPHERE:
            n = normalize(intersection - spheres[obj].pos);
            break;
        case TRIANGLE:
            n = tris[obj].n;
            break;
    }
    return n;
}

vec4 calcLighting(int prim, vec3 ray_o, vec3 intersection)
{
    Material m = materials[primitives[prim].material];
    vec4 color = m.col_diffuse * 0.2f;
    vec3 N = calcNormal(prim, intersection);
    for (int i = 0; i < pointlightCount; i++) {
        vec3 L = pointlights[i].pos - intersection;
        float dist = length(L);
        if (dot(N, L) < 0)
            break;
        Ray r = Ray(intersection, normalize(L));
        bool visible = true;
        for (int j = 0; j < primitiveCount; j++) {
            float t;
            switch (primitives[j].type) {
                case SPHERE:
                    t = intersectSphere(r, spheres[primitives[j].index]);
                    break;
                case TRIANGLE:
                    t = intersectTriangle(r, tris[primitives[j].index]);
                    break;
            }
            if (t > EPSILON && t < FAR_CLIP && t < dist) {
                visible = false;
                break;
            }
        }
        if (visible) {
            float att = 1 / (dist * dist);
            vec3 V = normalize(ray_o - intersection);
            vec3 R = normalize(reflect(-L, N));
            color += (1-m.spec) * dot(N, L) * m.col_diffuse * pointlights[i].color * att;
            color += m.spec * pow(max(0, dot(V, R)), 34) * m.col_specular;
        }
    }

    return color;
}

void main()
{
    ivec2 frag_coord = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(0, 0, 0, 1);

    vec3 screen_pos = p0 + p0p1 * (float(frag_coord.x) / screen_width)
                         + p0p2 * (float(frag_coord.y) / screen_height);
    vec3 ray_dir = normalize(screen_pos - cam_pos);

    Ray r = Ray(cam_pos, ray_dir);

    for (int j = 0; j < MAX_REFLECTIONS; j++) {
        float min_t = FAR_CLIP;
        int min_idx = 0;
        for (int i = 0; i < primitiveCount; i++) {
            float t = FAR_CLIP;
            switch (primitives[i].type) {
                case SPHERE:
                    t = intersectSphere(r, spheres[primitives[i].index]);
                    break;
                case TRIANGLE:
                    t = intersectTriangle(r, tris[primitives[i].index]);
            }

            if (t < min_t + EPSILON && t > EPSILON && t < FAR_CLIP) {
                min_t = t;
                min_idx = i;
            }
        }

        if (min_t < FAR_CLIP) {
            vec3 intersection = r.origin + min_t * r.dir;
            float refl = materials[primitives[min_idx].material].refl;
            if (refl == 0) {
                color += calcLighting(min_idx, r.origin, intersection);
                break;
            }
            color += refl * calcLighting(min_idx, r.origin, intersection);
            r.origin = intersection;
            r.dir = normalize(reflect(r.dir, calcNormal(min_idx, intersection)));
        }
        else {
            break;
        }
    }

    imageStore(dest_tex, frag_coord, color);
}
