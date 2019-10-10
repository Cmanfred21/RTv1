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

Vec3d   reflect(Vec3d const & I, Vec3d const & N) {
    return I - N * 2. * (I * N);
}

bool    scene_intersect(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures, Vec3d & hit, Vec3d & N, Material & material)
{
    double  dist = std::numeric_limits<double>::max();
    double  dist_i;
    for (auto i : figures)
    {
        if (i->if_intersect(orig, dir, dist_i) && dist_i < dist) {
            dist = dist_i;
            hit = orig + dir * dist_i;
            N = i->get_normal(hit);
            material = i->material;
        }
    }
    return dist < 1000;
}

Vec3d   cast_ray(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures, std::vector<Light> const & lights, size_t depth = 0)
{
    Vec3d       hit, N;
    Material    material;

    if (depth > 4 || !scene_intersect(orig, dir, figures, hit, N, material))
        return (Vec3d(0.2, 0.7, 0.8));

    Vec3d   reflect_dir = reflect(dir, N).normalize();
    Vec3d   reflect_orig = reflect_dir * N < 0 ?  hit - N * 1e-3 : hit + N * 1e-3;
    Vec3d   reflect_color = cast_ray(reflect_orig, reflect_dir, figures, lights, depth + 1);

    double  light_intensity = 0;
    double  spec_light_intensity = 0;
    for (auto it : lights) {
        Vec3d   light_dir = (it.position - hit).normalize();
        double  light_dist = (it.position - hit).norm();
        Vec3d   shadow_orig = light_dir * N < 0 ? hit - N * 1e-3 : hit + N * 1e-3;
        Vec3d   shadow_hit, shadow_N;
        Material    tmp_mat;
        if (scene_intersect(shadow_orig, light_dir, figures, shadow_hit, shadow_N, tmp_mat) && (shadow_hit - shadow_orig).norm() < light_dist)
            continue ;
        light_intensity += it.intensity * std::max(0., light_dir * N);
        spec_light_intensity += pow(std::max(0., -reflect(-light_dir, N) * dir), material.spec_exp) * it.intensity;
    }
    return material.color * light_intensity * material.albedo[0] + Vec3d(1., 1., 1.) * spec_light_intensity * material.albedo[1] + reflect_color * material.albedo[2];
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
    Material      ivory(Vec3d(0.4, 0.4, 0.3), Vec3d(0.6, 0.3, 0.1), 50.);
    Material      glass(Vec3d(0.6, 0.7, 0.8), Vec3d(0.5, 0.5, 0.5), 125.);
    Material red_rubber(Vec3d(0.3, 0.1, 0.1), Vec3d(0.9, 0.1, 0.0), 10.);
    Material     mirror(Vec3d(1., 1., 1.), Vec3d(0.0, 10.0, 0.8), 1425.);

    std::vector<Shape *> figures;

    figures.push_back(new Sphere(Vec3d(-3, 0, -16), 2, ivory));
    figures.push_back(new Sphere(Vec3d(-1.0, -1.5, -12), 2,      glass));
    figures.push_back(new Sphere(Vec3d(1.5, -0.5, -18), 3, red_rubber));
    figures.push_back(new Sphere(Vec3d( 7,    5,   -18), 4,     mirror));

    std::vector<Light>  lights;

    lights.push_back(Light(Vec3d(-20, 20, 20), 1.5));
    lights.push_back(Light(Vec3d( 30, 50, -25), 1.8));
    lights.push_back(Light(Vec3d( 30, 20,  30), 1.7));

    render(figures, lights);
    system("open ~/RTv1/out.ppm");
	return (0);
}
