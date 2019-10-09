#include "vect.h"
#include "rtv1.h"
#include <iostream>
#include <vector>
#include <fstream>

void    write_to_ppm(std::vector<Vec3d> framebuffer) {
    std::ofstream   ofs;
    ofs.open("./out.ppm");
    ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for(size_t i = 0; i < WIDTH * HEIGHT; i++)
        for (size_t j = 0; j < 3; j++)
            ofs << static_cast<char>(255 * std::max(0., std::min(1., framebuffer[i][j])));
}

bool    scene_intersect(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures, Vec3d & hit, Vec3d & N, Material & material)
{
    double  dist = std::numeric_limits<double>::max();
    double  dist_i;
    for (auto i : figures)
    {
//        std::cout << "lol" <<  std::endl;
        if (i->if_intersect(orig, dir, dist_i) && dist_i < dist) {
//            std::cout << "kek" <<  std::endl;
            dist = dist_i;
            hit = orig + dir * dist_i;
            N = i->get_normal(hit);
            material = i->material;
//            std::cout << material.color << std::endl;
        }
    }
    return dist < 1000;
}

Vec3d   cast_ray(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures, std::vector<Light> const & lights)
{
    Vec3d       hit, N;
    Material    material;

    if (!scene_intersect(orig, dir, figures, hit, N, material))
        return (Vec3d(0.2, 0.7, 0.8));
    double  light_intensity = 0;
    for (auto it : lights) {
        Vec3d   light_dir = (it.position - hit).normalize();
            std::cout << light_dir * N << std::endl;
        light_intensity += it.intensity * std::max(0., light_dir * N);
    }
    return material.color * light_intensity;
}

void    render(std::vector<Shape *> const & figures, std::vector<Light> const & lights)
{
    std::vector<Vec3d> framebuffer(WIDTH * HEIGHT);
    for (size_t j = 0; j < HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i < WIDTH; i++) {
            double  x =  (2 * (i + 0.5) / static_cast<double>(WIDTH)  - 1) * tan(FOV/2.) * WIDTH / static_cast<double>(HEIGHT);
            double  y = -(2 * (j + 0.5) / static_cast<double>(HEIGHT) - 1) * tan(FOV / 2.);
            Vec3d   dir = Vec3d(x, y, -1).normalize();
            framebuffer[ i + j * WIDTH] = cast_ray(Vec3d(0,0,0), dir, figures, lights);
        }
    }
    write_to_ppm(framebuffer);
}

int		main()
{
    Material      ivory(Vec3d(0.4, 0.4, 0.3));
    Material      glass(Vec3d(0.6, 0.7, 0.8));
    Material red_rubber(Vec3d(0.3, 0.1, 0.1));
    Material     mirror(Vec3d(1., 1., 1.));

    std::vector<Shape *> figures;

    figures.push_back(new Sphere(Vec3d(-3, 0, -16), 2, ivory));
    figures.push_back(new Sphere(Vec3d(-1.0, -1.5, -12), 2,      glass));
    figures.push_back(new Sphere(Vec3d(1.5, -0.5, -18), 3, red_rubber));
    figures.push_back(new Sphere(Vec3d( 7,    5,   -18), 4,     mirror));

    std::vector<Light>  lights;

    lights.push_back(Light(Vec3d(-20, 20, 20), 1.5));

    render(figures, lights);
    system("open ~/RTv1/out.ppm");
	return (0);
}
