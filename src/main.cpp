#include "vect.h"
#include "rtv1.h"
#include <iostream>
#include <vector>
#include <fstream>

void    write_to_bmp(std::vector<Vec3d> framebuffer) {
    // mimeType = "image/bmp";
    std::ofstream stream;
    stream.open("out.bmp");

    unsigned char file[14] = {
        'B','M', // magic
        0,0,0,0, // size in bytes
        0,0, // app data
        0,0, // app data
        40 + 14,0,0,0 // start of data offset
    };
    unsigned char info[40] = {
        40,0,0,0, // info hd size
        0,0,0,0, // width
        0,0,0,0, // heigth
        1,0, // number color planes
        24,0, // bits per pixel
        0,0,0,0, // compression is none
        0,0,0,0, // image bits size
        0,0,0,0,
        0,0,0,0,
        //0x13,0x0B,0,0, // horz resoluition in pixel / m
        //0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        0,0,0,0, // #colors in pallete
        0,0,0,0, // #important colors
    };

    int w = WIDTH;
    int h = HEIGHT;

    //int padSize = (4 - 3 * w % 4) % 4;
    int padSize = (4 - (w * 3) % 4) % 4;
    int sizeData = w * h * 3 + h * padSize;
    int sizeAll = sizeData + sizeof(file) + sizeof(info);

    file[2] = (unsigned char)(sizeAll);
    file[3] = (unsigned char)(sizeAll >> 8);
    file[4] = (unsigned char)(sizeAll >> 16);
    file[5] = (unsigned char)(sizeAll >> 24);

    info[4] = (unsigned char)(w);
    info[5] = (unsigned char)(w >> 8);
    info[6] = (unsigned char)(w >> 16);
    info[7] = (unsigned char)(w >> 24);

    info[8] = (unsigned char)(h);
    info[9] = (unsigned char)(h >> 8);
    info[10] = (unsigned char)(h >> 16);
    info[11] = (unsigned char)(h >> 24);

    info[20] = (unsigned char)(sizeData);
    info[21] = (unsigned char)(sizeData >> 8);
    info[22] = (unsigned char)(sizeData >> 16);
    info[23] = (unsigned char)(sizeData >> 24);

    stream.write((char*)file, sizeof(file));
    stream.write((char*)info, sizeof(info));

    unsigned char pad[3] = { 0,0,0 };

    for (auto& frame : framebuffer)
    {
        unsigned char pixel[3];
        pixel[0] = 255 * std::max(0., std::min(1., frame[0]));
        pixel[1] = 255 * std::max(0., std::min(1., frame[1]));
        pixel[2] = 255 * std::max(0., std::min(1., frame[2]));

        stream.write((char*)pixel, 3);
    }
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
    return dist < std::numeric_limits<double>::max();
}

Vec3d   cast_ray(Vec3d const & orig, Vec3d const & dir, std::vector<Shape *> const & figures, std::vector<Light> const & lights, size_t depth = 0)
{
    Vec3d       hit, N;
    Material    material;

    if (depth > 4 || !scene_intersect(orig, dir, figures, hit, N, material))
        return (Vec3d(0.3, 0.3, 0.3));

    Vec3d   reflect_dir = reflect(dir, N).normalize();
    Vec3d   reflect_orig = reflect_dir * N < 0 ?  hit - N * 1e-3 : hit + N * 1e-3;
    Vec3d   reflect_color = cast_ray(reflect_orig, reflect_dir, figures, lights, depth + 1);

    double  light_intensity = 0;
    double  spec_light_intensity = 0;
    for (auto& it : lights) {
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
    //return Vec3d(10.0, 0.0, -10.0);
    return material.color * light_intensity * material.albedo[0] + Vec3d(1., 1., 1.) * spec_light_intensity * material.albedo[1] + reflect_color * material.albedo[2];
}

void    render(std::vector<Shape *> const & figures, std::vector<Light> const & lights)
{
    std::vector<Vec3d> framebuffer(WIDTH * HEIGHT);
    for (size_t j = 0; j < HEIGHT; j++) { // actual rendering loop
        for (size_t i = 0; i < WIDTH; i++) {
            double dir_x = (i + 0.5) - WIDTH / 2.;
            double dir_y = -(j + 0.5) + HEIGHT / 2.;    // this flips the image at the same time
            double dir_z = -HEIGHT / (2. * std::tan(FOV / 2.));
            framebuffer[i + j * WIDTH] = cast_ray(Vec3d(0, 0, 0), Vec3d(dir_x, dir_y, dir_z).normalize(), figures, lights);
        }
    }
    write_to_bmp(framebuffer);
}

int		main()
{
    Material      ivory(Vec3d(0.4, 0.4, 0.3), Vec3d(0.6, 0.3, 0.1), 50.);
    Material      glass(Vec3d(0.6, 0.7, 0.8), Vec3d(0.5, 0.5, 0.5), 125.);
    Material blue_rubber(Vec3d(0.9, 0.1, 0.1), Vec3d(0.9, 0.1, 0.), 10.);
    Material     mirror(Vec3d(1., 1., 1.), Vec3d(0.0, 10.0, 0.8), 1425.);
    Material red_wall(Vec3d(0.1, 0.1, 0.3), Vec3d(0.9, 0.1, 0.), 10.);

    std::vector<Shape *> figures;
    std::vector<double> plane_formula{ 0., 1., 0., 100};

    figures.push_back(new Sphere(Vec3d(-3, 0, -10), 2, ivory));
    figures.push_back(new Sphere(Vec3d(-1.0, -4, -12), 2,      glass));
    figures.push_back(new Sphere(Vec3d(1.5, -0.5, -18), 3, blue_rubber));
    figures.push_back(new Sphere(Vec3d( 7,    5,   -18), 4,     mirror));
    figures.push_back(new Plane(vec<4, double>(plane_formula), red_wall));

    std::vector<Light>  lights;

    lights.push_back(Light(Vec3d(-20, 20, 20), 1.5));
    lights.push_back(Light(Vec3d( 30, 50, -25), 1.8));
    lights.push_back(Light(Vec3d( 30, 20,  30), 1.7));

    render(figures, lights);
	return (0);
}
