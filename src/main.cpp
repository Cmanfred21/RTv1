#include "vect.h"
#include "rtv1.h"
#include <iostream>
#include <vector>
#include <fstream>

Vec3f   cast_ray(Vec3f const & )

void    render(std::vector<Shape *> const & figures)
{
    std::vector<Vec3f> framebuffer(WIDTH * HEIGHT);
    for (size_t j = 0; j < HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i < WIDTH; i++) {
            float dir_x =  (i + 0.5f) -  WIDTH / 2.f;
            float dir_y = -(j + 0.5f) + HEIGHT / 2.f;    // this flips the image at the same time
            float dir_z = -HEIGHT / (2.f * tanf(static_cast<float>(FOV) / 2.f));
            framebuffer[ i + j * WIDTH] = cast_ray(Vec3f(0,0,0), Vec3f(dir_x, dir_y, dir_z), figures);
        }
    }
}

int		main()
{
    Material      ivory(Vec3f(0.4f, 0.4f, 0.3f));
    Material      glass(Vec3f(0.6f, 0.7f, 0.8f));
    Material red_rubber(Vec3f(0.3f, 0.1f, 0.1f));
    Material     mirror(Vec3f(1.f, 1.f, 1.f));

    std::vector<Shape *> figures;

    figures.push_back(new Sphere(Vec3f(-3, 0, -16), 2, ivory));
    figures.push_back(new Sphere(Vec3f(-1.0, -1.5, -12), 2,      glass));
    figures.push_back(new Sphere(Vec3f( 1.5, -0.5, -18), 3, red_rubber));
    figures.push_back(new Sphere(Vec3f( 7,    5,   -18), 4,     mirror));

    render(figures);
    system("open ~/RTv1/out.ppm");
	return (0);
}
