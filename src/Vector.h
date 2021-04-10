#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>


namespace math
{
    //
    // Vec4
    //
    class Vec4
    {
    public:
	float x, y, z, w;

	Vec4();
	Vec4(float n);
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec4& other);

	void set(float val[4]);

	void set(float x, float y, float z, float w);

	float* get();

	float* get() const;

	static Vec4& normalize(Vec4& v);

	Vec4& normalize();

	float length();

	float distance(Vec4& other);

	static float distance(Vec4& a, Vec4& b);

	float dot(Vec4& rhs);

	static float dot(Vec4& left, Vec4& right);

	Vec4& cross(Vec4& rhs);

	static Vec4& cross(Vec4& left, Vec4& right);

	float operator[](int idx);
	float operator[](int idx) const;
	Vec4& operator-();
	Vec4& operator=(const Vec4& other);
	Vec4& operator+=(const Vec4& rhs);
	Vec4& operator+=(float n);
	Vec4& operator-=(const  Vec4& rhs);
	Vec4& operator-=(float n);
	Vec4& operator*=(const Vec4& rhs);
	Vec4& operator*=(float n);
	Vec4& operator/=(const Vec4& rhs);
	Vec4& operator/=(float n);
    private:
	float values[4];
    };

    Vec4& operator+(const Vec4& a, const Vec4& b);
    Vec4& operator+(const Vec4& v, float n);
    Vec4& operator-(const Vec4& a, const Vec4& b);
    Vec4& operator-(const Vec4& v, float n);
    Vec4& operator*(const Vec4& a, const Vec4& n);
    Vec4& operator*(const Vec4& v, float n);
    Vec4& operator*(float n, const Vec4& v);
    Vec4& operator/(const Vec4& a, const Vec4& b);
    Vec4& operator/(const Vec4& v, float n);

    // Vec4 class alias
    using v4   = Vec4;
    using vec4 = Vec4;


    //
    // Vec3
    //
    class Vec3
    {
    public:
	float x, y, z;

	static const Vec3 up;		// (0, 1,  0)
	static const Vec3 right;	// (1, 0,  0)
	static const Vec3 forward;	// (0, 0, -1)

	Vec3();
	Vec3(float n);
	Vec3(float x, float y, float z);
	Vec3(const Vec3& other);

	void set(float val[3]);

	void set(float x, float y, float z);

	float* get();

	float* get() const;

	static Vec3& normalize(Vec3& v);

	Vec3& normalize();

	float length();

	float distance(Vec3& other);

	static float distance(Vec3& a, Vec3& b);

	float dot(Vec3& rhs);

	static float dot(Vec3& left, Vec3& right);

	Vec3& cross(Vec3& rhs);

	static Vec3& cross(Vec3& left, Vec3& right);

	float operator[](int idx);
	float operator[](int idx) const;
	Vec3& operator-();
	Vec3& operator=(const Vec3& other);
	Vec3& operator+=(const Vec3& rhs);
	Vec3& operator+=(float n);
	Vec3& operator-=(const Vec3& rhs);
	Vec3& operator-=(float n);
	Vec3& operator*=(const Vec3& rhs);
	Vec3& operator*=(float n);
	Vec3& operator/=(const Vec3& rhs);
	Vec3& operator/=(float n);
    private:
	float values[3];
    };

    Vec3& operator+(const Vec3& a, const Vec3& b);
    Vec3& operator+(const Vec3& v, float n);
    Vec3& operator-(const Vec3& a, const Vec3& b);
    Vec3& operator-(const Vec3& v, float n);
    Vec3& operator*(const Vec3& a, const Vec3& n);
    Vec3& operator*(const Vec3& v, float n);
    Vec3& operator*(float n, const Vec3& v);
    Vec3& operator/(const Vec3& a, const Vec3& b);
    Vec3& operator/(const Vec3& v, float n);

    //  Vec3 class alias
    using v3   = Vec3;
    using vec3 = Vec3;

    //
    // Vec2
    //
    class Vec2
    {
    public:
	float x, y;

	Vec2();
	Vec2(float n);
	Vec2(float x, float y);
	Vec2(const Vec2& other);

	void set(float val[2]);

	void set(float x, float y);

	float* get();

	float* get() const;

	static Vec2& normalize(Vec2& v);

	Vec2& normalize();

	float length();

	float distance(Vec2& other);

	static float distance(Vec2& a, Vec2& b);

	float dot(Vec2& rhs);

	static float dot(Vec2& left, Vec2& right);

	Vec2& cross(Vec2& rhs);

	static Vec2& cross(Vec2& left, Vec2& right);

	float operator[](int idx);
	float operator[](int idx) const;
	Vec2& operator-();
	Vec2& operator=(const Vec2& other);
	Vec2& operator+=(const Vec2& rhs);
	Vec2& operator+=(float n);
	Vec2& operator-=(const Vec2& rhs);
	Vec2& operator-=(float n);
	Vec2& operator*=(const Vec2& rhs);
	Vec2& operator*=(float n);
	Vec2& operator/=(const Vec2& rhs);
	Vec2& operator/=(float n);
    private:
	float values[2];
    };

    Vec2& operator+(const Vec2& a, const Vec2& b);
    Vec2& operator+(const Vec2& v, float n);
    Vec2& operator-(const Vec2& a, const Vec2& b);
    Vec2& operator-(const Vec2& v, float n);
    Vec2& operator*(const Vec2& a, const Vec2& n);
    Vec2& operator*(const Vec2& v, float n);
    Vec2& operator*(float n, const Vec2& v);
    Vec2& operator/(const Vec2& a, const Vec2& b);
    Vec2& operator/(const Vec2& v, float n);

    // Vec2 class alias
    using v2   = Vec2;
    using vec2 = Vec2;

}

#endif
