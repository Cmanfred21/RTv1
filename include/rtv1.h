#ifndef RTV1_H
# define RTV1_H

# include "vect.h"
# define WIDTH 1920
# define HEIGHT 1080

# define FOV M_PI / 2

struct  Material
{
public:
    Material(Vec3f const & v) : color(v) { }
    Vec3f   color;
};

struct  Shape
{
    Shape(Material const & m) : material(m) { }
    virtual ~Shape() {}
    virtual bool    if_intersect(Vec3f const & v, Vec3f const & point, float & dist) const noexcept = 0;
    Material    material;
};


struct  Sphere : Shape
{
public:
    Sphere(Vec3f const & c, float r, Material const & m) noexcept : Shape(m), center(c), radius(r) { }
    bool   if_intersect(Vec3f const & v, Vec3f const & point, float & dist) const noexcept;
private:
    Vec3f       center;
    float       radius;
};

#endif
