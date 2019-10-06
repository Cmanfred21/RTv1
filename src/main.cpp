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
            ofs << static_cast<char>(255 * framebuffer[i][j]);
}

bool    scene_intersect(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures, Vec3d & hit, Vec3d & N, Material material)
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
        }
    }
    return dist < 1000;
}

Vec3d   cast_ray(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures)
{
    Vec3d       hit, N;
    Material    material;

    if (!scene_intersect(orig, dir, figures, hit, N, material))
        return (Vec3d(0.2, 0.7, 0.8));
    return material.color;
}

void    render(std::vector<Shape *> const & figures)
{
    std::vector<Vec3d> framebuffer(WIDTH * HEIGHT);
    for (size_t j = 0; j < HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i < WIDTH; i++) {
            double dir_x =  (i + 0.5) -  WIDTH / 2.;
            double dir_y = -(j + 0.5) + HEIGHT / 2.;    // this flips the image at the same time
            double dir_z = -HEIGHT / (2. * tan(static_cast<double>(FOV) / 2.));
            framebuffer[ i + j * WIDTH] = cast_ray(Vec3d(0,0,0), Vec3d(dir_x, dir_y, dir_z).normalize(), figures);
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
    figures.push_back(new Sphere(Vec3d( 1.5, -0.5, -18), 3, red_rubber));
    figures.push_back(new Sphere(Vec3d( 7,    5,   -18), 4,     mirror));

    render(figures);
    system("open ~/RTv1/out.ppm");
	return (0);
}
