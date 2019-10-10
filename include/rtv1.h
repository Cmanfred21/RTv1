#ifndef RTV1_H
# define RTV1_H

# include "vect.h"
# define WIDTH 1920
# define HEIGHT 1080

# define FOV M_PI / 2

struct  Material
{
public:
    Material(Vec3d const & v, Vec3d const & a, double e) : color(v), albedo(a), spec_exp(e) { }
    Material() : color(), albedo(1, 0, 0), spec_exp() { }
    Vec3d   color;
    Vec3d   albedo;
    double  spec_exp;
};

struct  Shape
{
    Shape(Material const & m) : material(m) { }
    virtual ~Shape() {}
    virtual bool    if_intersect(Vec3d const & point, Vec3d const & v, double & dist) const noexcept = 0;
    virtual Vec3d & get_normal(Vec3d const & point) = 0;
    Material    material;
};


struct  Sphere : Shape
{
public:
    Sphere(Vec3d const & c, double r, Material const & m) noexcept : Shape(m), center(c), radius(r) { }
    bool    if_intersect(Vec3d const & point, Vec3d const & v, double & dist) const noexcept;
    Vec3d & get_normal(Vec3d const & point);
private:
    Vec3d       center;
    double      radius;
};

struct  Light
{
    Light(Vec3d const & p, const double i) : position(p), intensity (i) { }

    Vec3d   position;
    double  intensity;
};

#endif
