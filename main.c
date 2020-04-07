#include <stdio.h>
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

vec3 normalize(const vec3 v) {
    double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    vec3 unit = {
            .x = v.x / len,
            .y = v.y / len,
            .z = v.z / len
    };
    return unit;
}

// ----------------------------------------------------------------------------
// ray
// ----------------------------------------------------------------------------

typedef struct ray {
    vec3 orig;
    vec3 dir;
} ray;

vec3 ray_at(const ray ray, double t) {
    vec3 at = {
            .x = ray.orig.x + t * ray.dir.x,
            .y = ray.orig.y + t * ray.dir.y,
            .z = ray.orig.z + t * ray.dir.z
    };
    return at;
}

vec3 ray_color(const ray ray) {
    vec3 dir = normalize(ray.dir);
    double t = 0.5 * (dir.y + 1.0);
    vec3 color = {
            .r = (1.0 - t) * 1.0 + t * 0.5,
            .g = (1.0 - t) * 1.0 + t * 0.7,
            .b = (1.0 - t) * 1.0 + t * 1.0
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

    const int image_width = 200;
    const int image_height = 100;

    fprintf(file, "P3\n%d %d\n255\n", image_width, image_height);

    vec3 lower_left_corner = { .x = -2.0, .y = -1.0, .z = -1.0 };
    vec3 horizontal = { .x = 2.0, .y = 0.0, .z = 0.0 };
    vec3 vertical = { .x = 0.0, .y = 2.0, .z = 0.0 };
    vec3 origin = { .x = 0.0, .y = 0.0, .z = 0.0 };
    for (int j = image_height - 1; j >= 0; --j) {
        printf("\rScanlines remaining: %d ", j);
        for (int i = 0; i < image_width; ++i) {
            double u = (double) i / image_width;
            double v = (double) j / image_height;
            ray ray = {
                    .orig = origin,
                    .dir = {
                            .x = lower_left_corner.x + u * horizontal.x + v * vertical.x,
                            .y = lower_left_corner.y + u * horizontal.y + v * vertical.y,
                            .z = lower_left_corner.z + u * horizontal.z + v * vertical.z,
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
