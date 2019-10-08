#ifndef VECT_H
# define VECT_H

# include <cmath>
# include <cassert>
#include <iostream>

template <size_t DIM, typename T>
struct vec
{
	vec()  { for (size_t i =  0; i < DIM; i++) data_[i] = T(); }
    T &			operator[](const size_t i) noexcept { assert(i < DIM); return (data_[i]); }
    T const &	operator[](const size_t i) const noexcept { assert(i < DIM); return (data_[i]); }

private:
	T	data_[DIM];
};

typedef vec<2, float>	Vec2f;
typedef vec<2, int>		Vec2i;
typedef vec<3, float>	Vec3f;
typedef vec<3, int>		Vec3i;
typedef vec<3, double>	Vec3d;

template <typename T>
struct vec<2, T> {
	vec() : x(T()), y(T()) { }
	vec(T X, T Y) : x(X), y(Y) { }
    T &			operator[](const size_t i) noexcept { assert(i < 2 && i >= 0); return (i == 0 ? x : y); }
    T const &	operator[](const size_t i) const noexcept { assert(i < 2 && i >= 0); return (i == 0 ? x : y); }
	T x, y;
};

template <typename T>
struct vec<3, T>
{
	vec() : x(T()), y(T()), z(T()) { }
	vec(T X, T Y, T Z) : x(X), y(Y), z(Z) { }
    T &			operator[](const size_t i) noexcept { assert(i < 3 && i >= 0); return (i == 0 ? x : (i == 1 ? y : z)); }
    T const &	operator[](const size_t i) const noexcept { assert(i < 3 && i >= 0); return (i == 0 ? x : (i == 1 ? y : z)); }
    double      norm() { return std::sqrt(x * x + y * y + z * z); }
    vec<3, T> & normalize(T l = 1) { *this = (*this) * (l / norm()); return *this; }
	T x, y, z;
};

template <size_t DIM, typename T>
vec<DIM, T>     operator+(vec<DIM, T> const & lhs, vec<DIM, T> const & rhs) noexcept
{
    vec<DIM, T> ret;
    for (size_t i = 0; i < DIM; i++)
        ret[i] = lhs[i] + rhs[i];
    return (ret);
}

template <size_t DIM, typename T>
vec<DIM, T>     operator-(vec<DIM, T> const & lhs, vec<DIM, T> const & rhs) noexcept
{
    vec<DIM, T> ret;
    for (size_t i = 0; i < DIM; i++)
        ret[i] = lhs[i] - rhs[i];
    return (ret);
}

template<size_t DIM, typename T>
T               operator*(vec<DIM, T> const & lhs, vec<DIM, T> const & rhs)
{
    T ret = T();
    for (size_t i = 0; i < DIM; i++)
        ret += lhs[i] * rhs[i];
    return (ret);
}

template<size_t DIM, typename T, typename U>
vec<DIM, T>     operator*(vec<DIM, T> const & lhs, U const & rhs) noexcept
{
    vec<DIM, T> ret;
    for (size_t i = 0; i < DIM; i++)
        ret[i] = lhs[i] * rhs;
    return (ret);
}

//template<size_t DIM, typename T, typename U>
//vec<DIM, T>     operator*(U const & lhs, vec<DIM, T> const & rhs) noexcept
//{
//    vec<DIM, T> ret;
//    for (size_t i = 0; i < DIM; i++)
//        ret[i] += rhs[i] * lhs;
//    return (ret);
//}

template<size_t DIM, typename T>
vec<DIM, T>     operator-(vec<DIM, T> const & rhs) noexcept
{
    return (rhs * T(-1));
}

template<size_t DIM, typename T>
std::ostream &  operator<<(std::ostream & out, vec<DIM, T> const & v)
{
    for (size_t i = 0; i < DIM; i++)
        out << v[i] << ' ';
    return (out);
}

template<typename T>
vec<3, T>       cross(vec<3, T> const & v1, vec<3, T> const & v2)
{
    return (vec<3, T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x));
}

#endif
