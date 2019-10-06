#include "rtv1.h"

bool   Sphere::if_intersect(const Vec3d &point, const Vec3d &v, double & t0) const noexcept
{
//    std::cout << point << "           " << v << std::endl;
    Vec3d   dist = center - point;
    double  a = v * v;
    double  b = v * dist * 2;
    double  c = dist * dist - radius * radius;
    double  discr = b * b - 4 * a * c;
//    std::cout << a << " " << b << " " << c << std::endl;
    if (discr < 0)
        return (false);
    t0 = (-b + sqrt(discr)) / (2 * a);
    double t1 = (-b - sqrt(discr)) / (2 * a);
//    std::cout << t0 << "        " << t1 << "  " <<  a <<  std::endl;
    if (t0 < 0 &&  t1 < 0)
        return (false);
    t0 = t0 > t1 ? t0 : t1;
    return (true);
}

Vec3d & Sphere::get_normal(const Vec3d &point)
{
    return (point - center).normalize();
}
