#version 450

#define FAR_CLIP 10000.0f
#define EPSILON 0.0001f

#define SPHERE 0
#define PLANE 1

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

struct Plane
{
    vec3 n;
    float d;
};

/* Object buffers */

uniform int primitiveCount;

layout(std430, binding = 1) buffer primitiveBuffer { Primitive primitives[]; };
layout(std430, binding = 2) buffer sphereBuffer { Sphere spheres[]; };
layout(std430, binding = 3) buffer planeBuffer { Plane planes[]; };

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

float intersectPlane(Ray r, Plane p)
{
    float t = -dot(p.n, r.origin) + p.d;
    t /= dot(p.n, r.dir);
    if (t > EPSILON && t < FAR_CLIP)
        return t;
    return FAR_CLIP;
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
            case PLANE:
                t = intersectPlane(r, planes[primitives[i].index]);
                break;
        }

        if (t < min_t + EPSILON && t > EPSILON && t < FAR_CLIP) {
            min_t = t;
            min_idx = i;
        }
    }

    if (min_t < FAR_CLIP) {
        vec3 intersection = r.origin + min_t * r.dir;
        if (primitives[min_idx].type == PLANE)
            color = vec4(0.5f, 0.5f, 0.5f, 1);
        else
            color = vec4(1, 0, 0, 1);
    }

    imageStore(dest_tex, frag_coord, color);
}
