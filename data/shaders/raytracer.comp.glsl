#version 450

#define FAR_CLIP 10000.0f
#define EPSILON 0.0001f

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform writeonly image2D dest_tex;

uniform vec3 cam_pos;
uniform vec3 p0;
uniform vec3 p0p1;
uniform vec3 p0p2;

uniform int screen_width;
uniform int screen_height;

struct Ray
{
    vec3 origin;
    vec3 dir;
};

struct Sphere
{
    vec3 pos;
    float r;
};

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
        else return min(t1, t2);
    }
}

void main()
{
    ivec2 frag_coord = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    vec3 screen_pos = p0 + p0p1 * (float(frag_coord.x) / SCREEN_WIDTH)
                         + p0p2 * (float(frag_coord.y) / SCREEN_HEIGHT);
    vec3 ray_dir = normalize(screen_pos - cam_pos);

    Ray r = Ray(cam_pos, ray_dir);
    Sphere s = Sphere(vec3(0, 0, 0), 1);

    float t = intersectSphere(r, s);

    if (t < FAR_CLIP) {
        color = vec4(1, 1, 1, 1);
    }

    color = vec4(float(frag_coord.x) / SCREEN_WIDTH, float(frag_coord.y) / SCREEN_HEIGHT, 0, 1);

    imageStore(dest_tex, frag_coord, color);
}
