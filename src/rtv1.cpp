#include "rtv1.h"

bool   Sphere::if_intersect(const Vec3f &v, const Vec3f &point, float & dist) const noexcept
{
    Vec3f L = center - point;
    float tca = L * v;
    float d2 = L * L - tca * tca;
    if (d2 > radius * radius)
        return (false);
    float thc = sqrtf(radius * radius - d2);
    dist = tca - thc;
    float tmp = tca + thc;
    if (dist < 0)
        dist = tmp;
    if (dist < 0)
        return (false);
    return (true);
}
