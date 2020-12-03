#ifndef __CORE_GEOMETY_H__
#define __CORE_GEOMETY_H__

template <typename t>
struct Vec2{
	union {
		struct { t u, v; };
		struct { t x, y; };
		t raw[2];
	};
	Vec2():u(0),v(0){}
	Vec2(t u_, t v_) :x(u_), y(v_) {}
	inline Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u + V.u, v + V.v); }
	inline Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u - V.u, v - V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};


template <typename t>
struct Vec3 {
	union
	{
		struct {t x, y, z;};
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() :x(0), y(0), z(0) {};
	Vec3(t u, t v, t w) :x(u), y(v), z(w) {}
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
#endif // !__CORE_GEOMETY_H__
