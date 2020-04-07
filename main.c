#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// ----------------------------------------------------------------------------
// vec3
// ----------------------------------------------------------------------------

typedef struct vec3 {
    union {
        double elem[3];
        struct {
            double x, y, z;
        };
        struct {
            double r, g, b;
        };
    };
} vec3;

void vec_set(vec3 *vec, double x, double y, double z) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

double vec_length(const vec3 v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 vec_unit(const vec3 v) {
    double len = vec_length(v);
    vec3 unit = {
            v.x / len,
            v.y / len,
            v.z / len
    };
    return unit;
}

double vec_normalize(vec3 *v) {
    double len = vec_length(*v);
    if (len == 0.0) {
        vec_set(v, v->x / len, v->y / len, v->z / len);
    }
    return len;
}

void vec_inverse(vec3 *v) {
    v->x *= -1.0;
    v->y *= -1.0;
    v->z *= -1.0;
}

vec3 vec_subtract(const vec3 v1, const vec3 v2) {
    vec3 result = {
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z
    };
    return result;
}

vec3 vec_add(const vec3 v1, const vec3 v2) {
    vec3 result = {
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z
    };
    return result;
}

vec3 vec_scale(const vec3 v, double s) {
    vec3 result = {
            s * v.x,
            s * v.y,
            s * v.z
    };
    return result;
}

double vec_dot(const vec3 v1, const vec3 v2) {
    return sqrt(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}


// ----------------------------------------------------------------------------
// ray
// ----------------------------------------------------------------------------

typedef struct ray {
    vec3 orig;
    vec3 dir;
} ray;

vec3 ray_at(const ray ray, double t) {
    vec3 at = vec_add(ray.orig, vec_scale(ray.dir, t));
    return at;
}

// ----------------------------------------------------------------------------
// raytrace
// ----------------------------------------------------------------------------

bool hit_sphere(const vec3 center, double radius, const ray ray) {
    vec3 oc = vec_subtract(center, ray.orig);
    double a = vec_dot(ray.dir, ray.dir);
    double b = 2.0 * vec_dot(oc, ray.dir);
    double c = vec_dot(oc, oc) - radius*radius;
    double discriminant = b*b - 4*a*c;
    return (discriminant > 0.0);
}

vec3 ray_color(const ray ray) {
    vec3 center = { 0, 0, -1 };
    if (hit_sphere(center, 0.5, ray)) {
        return (vec3) {1, 0, 0};
    }

    vec3 dir = vec_unit(ray.dir);
    double t = 0.5 * (dir.y + 1.0);
    vec3 color = {
            (1.0 - t) * 1.0 + t * 0.5,
            (1.0 - t) * 1.0 + t * 0.7,
            (1.0 - t) * 1.0 + t * 1.0
    };
    return color;
}

// ----------------------------------------------------------------------------

void write_color(FILE *file, const vec3 color) {
    int ir = (int) (255.999 * color.r);
    int ig = (int) (255.999 * color.g);
    int ib = (int) (255.999 * color.b);
    fprintf(file, "%d %d %d\n", ir, ig, ib);
}


// ============================================================================
// main
// ============================================================================


int main() {
    const char *filename = "../output/image.ppm";
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open image.ppm for writing\n");
        return 1;
    }

    const int image_width = 400;
    const int image_height = 200;

    fprintf(file, "P3\n%d %d\n255\n", image_width, image_height);

    vec3 lower_left_corner = { -2, -1, -1 };
    vec3 horizontal        = {  4,  0,  0 };
    vec3 vertical          = {  0,  2,  0 };
    vec3 origin            = {  0,  0,  0 };

    for (int j = image_height - 1; j >= 0; --j) {
        printf("\rScanlines remaining: %d ", j);

        for (int i = 0; i < image_width; ++i) {
            double u = (double) i / image_width;
            double v = (double) j / image_height;
            ray ray = {
                    .orig = origin,
                    .dir = {
                        lower_left_corner.x + u * horizontal.x + v * vertical.x,
                        lower_left_corner.y + u * horizontal.y + v * vertical.y,
                        lower_left_corner.z + u * horizontal.z + v * vertical.z,
                    }
            };
            vec3 color = ray_color(ray);
            write_color(file, color);
        }
    }

    fclose(file);

    printf("\nDone.\n");

    return 0;
}
