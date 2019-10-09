#include "rtv1.h"

bool   Sphere::if_intersect(const Vec3d &point, const Vec3d &v, double & t0) const noexcept
{
    Vec3d   dist = center - point;
    double  b = v * dist;
    double  c = dist * dist - radius * radius;
    double  discr = b * b - c;
    if (discr < 0)
        return (false);
    t0 = b + sqrt(discr);
    double t1 = b - sqrt(discr);
    if (t0 < 0 &&  t1 < 0)
        return (false);
    if (t1 < t0 && t1 > 0)
        t0 = t1;
    return (true);
}

Vec3d & Sphere::get_normal(const Vec3d &point)
{
    return (point - center).normalize();
}
