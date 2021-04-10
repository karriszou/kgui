#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <iostream>

#include "Vector.h"


namespace math
{

    //
    // Column-major order 4x4 matrix
    //
    class Mat4
    {
    public:
	Mat4(bool isPreMultiply = true);	// Identity matrix
	Mat4(const Mat4& other);

	Mat4& identity();

	Mat4& transpose();

	Mat4& invert();

	Mat4& translate(float x, float y, float z);

	Mat4& translate(const Vec3& pos);

	Mat4& rotate(float angle, float x, float y, float z);

	Mat4& rotate(float angle, const Vec3& axis);

	Mat4& scale(float s);

	Mat4& scale(float x, float y, float z);

	Mat4& scale(const Vec3& scale);

	// Build a rotate matrix, assume up is (0, 1, 0).
	// It is for rotating object to look at the target, not for camera.
	// Note that the function clears the previous 3x3 rotation component of the matrix,
	// and re-computes it with target point.
	Mat4& lookAt(float tx, float ty, float tz);

	Mat4& lookAt(const Vec3& target);

	// Build a right-handed view matrix(no translation part).
	// Parameters: forward, up.
	Mat4& lookAt(float fx, float fy, float fz, float ux, float uy , float uz);

	Mat4& lookAt(const Vec3& forward, const Vec3& up);

	// Build a right-handed view matrix by eye(position), center(target), up vectors.
	// Parameters: position, position + forward, up.
	Mat4& lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy , float uz);

	Mat4& lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

	Mat4& lookAtLH(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy , float uz);

	Mat4& lookAtLH(const Vec3& eye, const Vec3& center, const Vec3& up);

	Mat4& frustum(float left, float right, float bottom, float top, float near, float far);

	// Parameters: verticle field-of-view radians, width / height ratio, near plane, far plane
	Mat4& perspective(float fovy, float aspect, float near, float far);

	Mat4& ortho(float left, float right, float bottom, float top, float near, float far);

	Mat4& set(float values[16]);

	float* get();

	float operator[](int idx);
	float operator[](int idx) const;

    private:
	bool isPreMultiply;
	float values[16];
    
    };

    Mat4& operator*(Mat4&lhs, Mat4& rhs);

    Mat4& operator*(Mat4& lhs, float rhs[16]);

    Mat4& operator*(float lhs[16], Mat4& rhs);

    std::ostream& operator<<(std::ostream& os, Mat4& rhs);

    // Class alias
    using m4   = Mat4;
    using mat4 = Mat4;
    
}

#endif
