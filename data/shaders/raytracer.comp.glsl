#version 450

#define FAR_CLIP 10000.0f
#define EPSILON 0.0001f

#define SPHERE 0
#define BOX 1

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
    int dielectric;
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

struct Box
{
    vec3 min;
    vec3 max;
};

/* Object buffers */

uniform int primitiveCount;
uniform int pointlightCount;

layout(std430, binding = 1) buffer materialBuffer { Material materials[]; };
layout(std430, binding = 2) buffer pointlightsBuffer { Pointlight pointlights[]; };
layout(std430, binding = 3) buffer primitiveBuffer { Primitive primitives[]; };
layout(std430, binding = 4) buffer sphereBuffer { Sphere spheres[]; };
layout(std430, binding = 5) buffer boxBuffer { Box boxes[]; };

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

float intersectBox(Ray r, Box b)
{
    float t1x = (b.min.x - r.origin.x) / r.dir.x;
    float t2x = (b.max.x - r.origin.x) / r.dir.x;

    float tmin = min(t1x, t2x);
    float tmax = max(t1x, t2x);

    float t1y = (b.min.y - r.origin.y) / r.dir.y;
    float t2y = (b.max.y - r.origin.y) / r.dir.y;

    tmin = max(tmin, min(t1y, t2y));
    tmax = min(tmax, max(t1y, t2y));

    float t1z = (b.min.z - r.origin.z) / r.dir.z;
    float t2z = (b.max.z - r.origin.z) / r.dir.z;

    tmin = max(tmin, min(t1z, t2z));
    tmax = min(tmax, max(t1z, t2z));

    if (tmax >= tmin)
        return tmin;
    return FAR_CLIP;
}

vec3 calcNormal(int prim, vec3 intersection)
{
    vec3 n;
    int obj = primitives[prim].index;
    switch (primitives[prim].type) {
        case SPHERE:
            n = normalize(intersection - spheres[obj].pos);
            break;
        case BOX:
            Box b = boxes[obj];
            vec3 c = (b.min + b.max) * 0.5f;
            vec3 size = vec3(abs(b.max.x - b.min.x),
                             abs(b.max.y - b.min.y),
                             abs(b.max.z - b.min.z));
            vec3 p = intersection - c;
            float dist = (size.x - abs(p.x));
            float min = dist;
            n = vec3(1, 0, 0) * sign(p.x);
            dist = (size.y - abs(p.y));
            if (dist < min) {
                min = dist;
                n = vec3(0, 1, 0) * sign(p.y);
            }
            dist = (size.z - abs(p.z));
            if (dist < min)
                n = vec3(0, 0, 1) * sign(p.z);
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
                case BOX:
                    t = intersectBox(r, boxes[primitives[j].index]);
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

    float min_t = FAR_CLIP;
    int min_idx = 0;
    for (int i = 0; i < primitiveCount; i++) {
        float t = FAR_CLIP;
        switch (primitives[i].type) {
            case SPHERE:
                t = intersectSphere(r, spheres[primitives[i].index]);
                break;
            case BOX:
                t = intersectBox(r, boxes[primitives[i].index]);
                break;
        }

        if (t < min_t + EPSILON && t > EPSILON && t < FAR_CLIP) {
            min_t = t;
            min_idx = i;
        }
    }

    if (min_t < FAR_CLIP) {
        vec3 intersection = r.origin + min_t * r.dir;
        color = calcLighting(min_idx, r.origin, intersection);
    }

    imageStore(dest_tex, frag_coord, color);
}
