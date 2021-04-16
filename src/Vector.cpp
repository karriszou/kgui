#include <cassert>
#include "Vector.h"


namespace math
{

    //
    // Vec4
    //
    Vec4::Vec4()
    {
	x = 0.0;
	y = 0.0;
	z = 0.0;
	w = 0.0;
    }

    Vec4::Vec4(float n)
    {
	x = n;
	y = n;
	z = n;
	w = n;
    }

    Vec4::Vec4(float x, float y, float z, float w)
    {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
    }

    Vec4::Vec4(const Vec4& other)
	:x(other.x),
	 y(other.y),
	 z(other.z),
	 w(other.w)
    {

    }

    void Vec4::set(float val[4])
    {
	x = val[0];
	y = val[1];
	z = val[2];
	w = val[3];
    }

    void Vec4::set(float x, float y, float z, float w)
    {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
    }

    float* Vec4::get()
    {
	values[0] = x;
	values[1] = y;
	values[2] = z;
	values[3] = w;
	return values;
    }

    float* Vec4::get() const
    {
	float *res = new float[4] { x, y, z, w };
	return res;
    }

    Vec4& Vec4::normalize()
    {
	float m = sqrtf(x * x + y * y + z * z + w * w);
	x /= m;
	y /= m;
	z /= m;
	w /= m;
	return *this;
    }

    Vec4& Vec4::normalize(Vec4& v)	// @Static
    {
	Vec4& res(v);
	float m = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	res.x /= m;
	res.y /= m;
	res.z /= m;
	res.w /= m;
	return res;
    }

    float Vec4::length()
    {
	return sqrtf(x * x + y * y + z * z + w * w);
    }

    float Vec4::distance(Vec4& other)
    {
	float dx = other.x - x;
	float dy = other.y - y;
	float dz = other.z - z;
	float dw = other.w - w;
	return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
    }

    float Vec4::distance(Vec4& a, Vec4& b)	// @Static
    {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	float dw = a.w - b.w;
	return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
    }

    float Vec4::dot(Vec4& rhs)
    {
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    float Vec4::dot(Vec4& left, Vec4& right)	// @Static
    {
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
    }

    Vec4& Vec4::cross(Vec4& rhs)
    {
	Vec4& res = *new Vec4();
	res.x = y * rhs.z - z * rhs.y;
	res.y = z * rhs.w - w * rhs.z;
	res.z = w * rhs.x - x * rhs.w; 
	res.w = x * rhs.y - y * rhs.x;
	return res;
    }

    Vec4& Vec4::cross(Vec4& left, Vec4& right)	// @Static
    {
	Vec4& res = *new Vec4();
	res.x = left.y * right.z - left.z * right.y;
	res.y = left.z * right.w - left.w * right.z;
	res.z = left.w * right.x - left.x * right.w; 
	res.w = left.x * right.y - left.y * right.x;
	return res;
    }

    float Vec4::operator[](int idx)
    {
	assert(idx >= 0 && idx < 4);
	values[0] = x;
	values[1] = y;
	values[2] = z;
	values[3] = w;
	return values[idx];
    }

    float Vec4::operator[](int idx) const
    {
	assert(idx >= 0 && idx < 4);
	if(idx == 0) return x;
	else if(idx == 1) return y;
	else if(idx == 2) return z;
	else if(idx == 3) return w;
	return x;
    }

    Vec4& Vec4::operator-()
    {
	return *new Vec4(-x, -y, -z, -w);
    }

    Vec4& Vec4::operator=(const Vec4& other)
    {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
    }

    Vec4& Vec4::operator+=(const Vec4& rhs)
    {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w += rhs.w;
	return *this;
    }

    Vec4& Vec4::operator+=(float n)
    {
	x += n;
	y += n;
	z += n;
	w += n;
	return *this;
    }

    Vec4& Vec4::operator-=(const Vec4& rhs)
    {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w -= rhs.w;
	return *this;
    }

    Vec4& Vec4::operator-=(float n)
    {
	x -= n;
	y -= n;
	z -= n;
	w -= n;
	return *this;
    }

    Vec4& Vec4::operator*=(const Vec4& rhs)
    {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	w *= rhs.w;
	return *this;
    }

    Vec4& Vec4::operator*=(float n)
    {
	x *= n;
	y *= n;
	z *= n;
	w *= n;
	return *this;
    }

    Vec4& Vec4::operator/=(const Vec4& rhs)
    {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	w /= rhs.w;
	return *this;
    }

    Vec4& Vec4::operator/=(float n)
    {
	x /= n;
	y /= n;
	z /= n;
	w /= n;
	return *this;
    }

    //
    // Free funtions
    //
    Vec4& operator+(const Vec4& a, const Vec4& b)
    {
	return *new Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    Vec4& operator+(const Vec4& v, float n)
    {
	return *new Vec4(v.x + n, v.y + n, v.z + n, v.w + n);
    }

    Vec4& operator-(const Vec4& a, const Vec4& b)
    {
	return *new Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    Vec4& operator-(const Vec4& v, float n)
    {
	return *new Vec4(v.x - n, v.y - n, v.z - n, v.w - n);
    }

    Vec4& operator*(const Vec4& a, const Vec4& b)
    {
	return *new Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    Vec4& operator*(const Vec4& v, float n)
    {
	return *new Vec4(n * v.x, n * v.y, n * v.z, n * v.w);
    }

    Vec4& operator*(float n, const Vec4& v)
    {
	return *new Vec4(v.x * n, v.y * n, v.z * n, v.w * n);
    }

    Vec4& operator/(const Vec4& a, const Vec4& b)
    {
	return *new Vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    Vec4& operator/(const Vec4& v, float n)
    {
	return *new Vec4(v.x / n, v.y / n, v.z / n, v.w / n);
    }


    //
    // Vec3
    //
    // const Vec3 Vec3::up	= Vec3(0, 1,  0);
    // const Vec3 Vec3::right	= Vec3(1, 0,  0);
    // const Vec3 Vec3::forward	= Vec3(0, 0, -1);
    
    Vec3::Vec3()
    {
	x = 0.0;
	y = 0.0;
	z = 0.0;
    }

    Vec3::Vec3(float n)
    {
	x = n;
	y = n;
	z = n;
    }

    Vec3::Vec3(float x, float y, float z)
    {
	this->x = x;
	this->y = y;
	this->z = z;
    }

    Vec3::Vec3(const Vec3& other)
	:x(other.x),
	 y(other.y),
	 z(other.z)
    {

    }

    void Vec3::set(float val[3])
    {
	x = val[0];
	y = val[1];
	z = val[2];
    }

    void Vec3::set(float x, float y, float z)
    {
	this->x = x;
	this->y = y;
	this->z = z;
    }

    float* Vec3::get()
    {
	values[0] = x;
	values[1] = y;
	values[2] = z;
	return values;
    }

    float* Vec3::get() const
    {
	float *res = new float[3] { x, y, z };
	return res;
    }

    Vec3& Vec3::normalize()
    {
	float m = sqrtf(x * x + y * y + z * z);
	x /= m;
	y /= m;
	z /= m;
	return *this;
    }

    Vec3& Vec3::normalize(Vec3& v)	// @Static
    {
	Vec3& res(v);
	float m = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	res.x /= m;
	res.y /= m;
	res.z /= m;
	return res;
    }

    float Vec3::length()
    {
	return sqrtf(x * x + y * y + z * z);
    }

    float Vec3::distance(Vec3& other)
    {
	float dx = other.x - x;
	float dy = other.y - y;
	float dz = other.z - z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    float Vec3::distance(Vec3& a, Vec3& b)	// @Static
    {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    float Vec3::dot(Vec3& rhs)
    {
	return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    float Vec3::dot(Vec3& left, Vec3& right)	// @Static
    {
	return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    Vec3& Vec3::cross(Vec3& rhs)
    {
	Vec3& res = *new Vec3();
	res.x = y * rhs.z - z * rhs.y;
	res.y = z * rhs.x - x * rhs.z;
	res.z = x * rhs.y - y * rhs.x; 
	return res;
    }

    Vec3& Vec3::cross(Vec3& left, Vec3& right)	// @Static
    {
	Vec3& res = *new Vec3();
	res.x = left.y * right.z - left.z * right.y;
	res.y = left.z * right.x - left.x * right.z;
	res.z = left.x * right.y - left.y * right.x; 
	return res;
    }

    Vec3& Vec3::up()
    {
	static Vec3 up = Vec3(0, 1, 0);
	return up;
    }

    Vec3& Vec3::right()
    {
	static Vec3 right = Vec3(1, 0, 0);
	return right;
    }

    Vec3& Vec3::forward()
    {
	static Vec3 forward = Vec3(0, 0, -1);
	return forward;
    }

    float Vec3::operator[](int idx)
    {
	assert(idx >= 0 && idx < 3);
	values[0] = x;
	values[1] = y;
	values[2] = z;
	return values[idx];
    }

    float Vec3::operator[](int idx) const
    {
	assert(idx >= 0 && idx < 3);
	if(idx == 0) return x;
	else if(idx == 1) return y;
	else if(idx == 2) return z;
	return x;
    }

    Vec3& Vec3::operator-()
    {
	return *new Vec3(-x, -y, -z);
    }

    Vec3& Vec3::operator=(const Vec3& other)
    {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
    }

    Vec3& Vec3::operator+=(const Vec3& rhs)
    {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
    }

    Vec3& Vec3::operator+=(float n)
    {
	x += n;
	y += n;
	z += n;
	return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& rhs)
    {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
    }

    Vec3& Vec3::operator-=(float n)
    {
	x -= n;
	y -= n;
	z -= n;
	return *this;
    }

    Vec3& Vec3::operator*=(const Vec3& rhs)
    {
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
    }

    Vec3& Vec3::operator*=(float n)
    {
	x *= n;
	y *= n;
	z *= n;
	return *this;
    }

    Vec3& Vec3::operator/=(const Vec3& rhs)
    {
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;
	return *this;
    }

    Vec3& Vec3::operator/=(float n)
    {
	x /= n;
	y /= n;
	z /= n;
	return *this;
    }

    //
    // Free funtions
    //
    Vec3& operator+(const Vec3& a, const Vec3& b)
    {
	return *new Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    Vec3& operator+(const Vec3& v, float n)
    {
	return *new Vec3(v.x + n, v.y + n, v.z + n);
    }

    Vec3& operator-(const Vec3& a, const Vec3& b)
    {
	return *new Vec3(a.x - b.x, a.y - b.y, a.z - b.z);    
    }

    Vec3& operator-(const Vec3& v, float n)
    {
	return *new Vec3(v.x - n, v.y - n, v.z - n);
    }

    Vec3& operator*(const Vec3& a, const Vec3& b)
    {
	return *new Vec3(a.x * b.x, a.y * b.y, a.z * b.z);    
    }

    Vec3& operator*(const Vec3& v, float n)
    {
	return *new Vec3(n * v.x, n * v.y, n * v.z);
    }

    Vec3& operator*(float n, const Vec3& v)
    {
	return *new Vec3(v.x * n, v.y * n, v.z * n);
    }

    Vec3& operator/(const Vec3& a, const Vec3& b)
    {
	return *new Vec3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    Vec3& operator/(const Vec3& v, float n)
    {
	return *new Vec3(v.x / n, v.y / n, v.z / n);
    }

    
    //
    // Vec2
    //
    Vec2::Vec2()
    {
	x = 0.0;
	y = 0.0;
    }

    Vec2::Vec2(float n)
    {
	x = n;
	y = n;
    }

    Vec2::Vec2(float x, float y)
    {
	this->x = x;
	this->y = y;
    }

    Vec2::Vec2(const Vec2& other)
	:x(other.x),
	 y(other.y)
    {

    }

    void Vec2::set(float val[2])
    {
	x = val[0];
	y = val[1];
    }

    void Vec2::set(float x, float y)
    {
	this->x = x;
	this->y = y;
    }
    
    float* Vec2::get()
    {
	values[0] = x;
	values[1] = y;
	return values;
    }

    float* Vec2::get() const
    {
	float *res = new float[2] { x, y };
	return res;
    }

    Vec2& Vec2::normalize()
    {
	float m = sqrtf(x * x + y * y);
	x /= m;
	y /= m;
	return *this;
    }

    Vec2& Vec2::normalize(Vec2& v)	// @Static
    {
	Vec2& res(v);
	float m = sqrtf(v.x * v.x + v.y * v.y);
	res.x /= m;
	res.y /= m;
	return res;
    }

    float Vec2::length()
    {
	return sqrtf(x * x + y * y);
    }

    float Vec2::distance(Vec2& other)
    {
	float dx = other.x - x;
	float dy = other.y - y;
	return sqrtf(dx * dx + dy * dy);
    }

    float Vec2::distance(Vec2& a, Vec2& b)	// @Static
    {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrtf(dx * dx + dy * dy);
    }

    float Vec2::dot(Vec2& rhs)
    {
	return x * rhs.x + y * rhs.y;
    }

    float Vec2::dot(Vec2& left, Vec2& right)	// @Static
    {
	return left.x * right.x + left.y * right.y;
    }

    Vec2& Vec2::cross(Vec2& rhs)
    {
	Vec2& res = *new Vec2();
	res.x = y * rhs.x - x * rhs.y;
	res.y = x * rhs.y - y * rhs.x;
	return res;
    }

    Vec2& Vec2::cross(Vec2& left, Vec2& right)	// @Static
    {
	Vec2& res = *new Vec2();
	res.x = left.y * right.x - left.x * right.y;
	res.y = left.x * right.y - left.y * right.x;
	return res;
    }

    float Vec2::operator[](int idx)
    {
	assert(idx >= 0 && idx < 2);
	values[0] = x;
	values[1] = y;
	return values[idx];
    }

    float Vec2::operator[](int idx) const
    {
	assert(idx >= 0 && idx < 2);
	if(idx == 0) return x;
	else if(idx == 1) return y;
	return x;
    }

    Vec2& Vec2::operator-()
    {
	return *new Vec2(-x, -y);
    }

    Vec2& Vec2::operator=(const Vec2& other)
    {
	x = other.x;
	y = other.y;
	return *this;
    }

    Vec2& Vec2::operator+=(const Vec2& rhs)
    {
	x += rhs.x;
	y += rhs.y;
	return *this;
    }

    Vec2& Vec2::operator+=(float n)
    {
	x += n;
	y += n;
	return *this;
    }

    Vec2& Vec2::operator-=(const Vec2& rhs)
    {
	x -= rhs.x;
	y -= rhs.y;
	return *this;
    }

    Vec2& Vec2::operator-=(float n)
    {
	x -= n;
	y -= n;
	return *this;
    }

    Vec2& Vec2::operator*=(const Vec2& rhs)
    {
	x *= rhs.x;
	y *= rhs.y;
	return *this;
    }

    Vec2& Vec2::operator*=(float n)
    {
	x *= n;
	y *= n;
	return *this;
    }

    Vec2& Vec2::operator/=(const Vec2& rhs)
    {
	x /= rhs.x;
	y /= rhs.y;
	return *this;
    }

    Vec2& Vec2::operator/=(float n)
    {
	x /= n;
	y /= n;
	return *this;
    }


    //
    // Free funtions
    //
    Vec2& operator+(const Vec2& a, const Vec2& b)
    {
	return *new Vec2(a.x + b.x, a.y + b.y);
    }

    Vec2& operator+(const Vec2& v, float n)
    {
	return *new Vec2(v.x + n, v.y + n);
    }

    Vec2& operator-(const Vec2& a, const Vec2& b)
    {
	return *new Vec2(a.x - b.x, a.y - b.y);
    }

    Vec2& operator-(const Vec2& v, float n)
    {
	return *new Vec2(v.x - n, v.y - n);
    }

    Vec2& operator*(const Vec2& a, const Vec2& b)
    {
	return *new Vec2(a.x * b.x, a.y * b.y);
    }

    Vec2& operator*(const Vec2& v, float n)
    {
	return *new Vec2(n * v.x, n * v.y);
    }

    Vec2& operator*(float n, const Vec2& v)
    {
	return *new Vec2(v.x * n, v.y * n);
    }

    Vec2& operator/(const Vec2& a, const Vec2& b)
    {
	return *new Vec2(a.x / b.x, a.y / b.y);
    }

    Vec2& operator/(const Vec2& v, float n)
    {
	return *new Vec2(v.x / n, v.y / n);
    }
    
    
}	// namespace math
