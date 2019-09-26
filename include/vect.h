#ifndef VECT_H
# define VECT_H

# include <cmath>
# include <cassert>

template <size_t DIM, typename T>
struct vec  {
	vec()  { for (size_t i =  0; i < DIM; i++) data_[i] = T(); }
	T &			operator[](const size_t i) noexcept { assert(i < DIM); return (data[i]); }
	T const &	operator[](const size_t i) const noexcept { assert(i < DIM); return (data[i]); }

private:
	T	data_[DIM];
};

typedef vec<2, float>	Vec2f;
typedef vec<2, int>		Vec2i;
typedef vec<3, float>	Vec3f;
typedef vec<3, int>		Vec3i;

template <typename T>
struct vec<2, T> {
	vec() : x(T()), y(T()) { }
	vec(T X, T Y) : x(X), y(Y) { }
	T &			operator[](const size_t i) noexcept { assert(i < 2 && i > 0); return (i == 0 ? x : y); }
	T const &	operator[](const size_t i) const noexcept { assert(i < 2 && i > 0); return (i == 0 ? x : y); }
	T x, y;
};

template <typename T>
struct vec<3, T>
{
	vec() : x(T()), y(T()), z(T()) { }
	vec(T X, T Y, T Z) : x(X), y(Y), z(Z) { }
	T &			operator[](const size_t i) noexcept { assert(i < 3 && i > 0); return (i == 0 ? x : (i == 1 ? y : z)); }
	T const &	operator[](const size_t i) const noexcept { assert(i < 3 && i > 0); return (i == 0 ? x : (i == 1 ? y : z)); }
	T x, y, z;
};


#endif