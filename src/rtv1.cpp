#include "rtv1.h"

bool   Sphere::if_intersect(const Vec3d &point, const Vec3d &v, double & t0) const
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

bool   Plane::if_intersect(const Vec3d& point, const Vec3d& v, double& t0) const
{
    double k = -(point[0] * koef[0] + point[1] * koef[1] + point[2] * koef[2] + koef[3]) / (koef[0] * v[0] + koef[1] * v[1] + koef[2] * v[2]);
    t0 = (v * k).Length();
    return k >= 0;
}

Vec3d Sphere::get_normal(const Vec3d &point)
{
    return (point - center).normalize();
}

Vec3d Plane::get_normal(Vec3d const&)
{
    return Vec3d(koef[0], koef[1], koef[2]);
}
