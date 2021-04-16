#include <iomanip>
#include <algorithm>
#include <cassert>
#include "Matrix.h"


namespace math
{


    Mat4::Mat4(bool isPreMultiply)
	:isPreMultiply(isPreMultiply)
    {
	values[0]  = 1.0;
	values[1]  = 0.0;
	values[2]  = 0.0;
	values[3]  = 0.0;

	values[4]  = 0.0;
	values[5]  = 1.0;
	values[6]  = 0.0;
	values[7]  = 0.0;

	values[8]  = 0.0;
	values[9]  = 0.0;
	values[10] = 1.0;
	values[11] = 0.0;

	values[12] = 0.0;
	values[13] = 0.0;
	values[14] = 0.0;
	values[15] = 1.0;
    }

    Mat4::Mat4(const Mat4& other)
    {
	values[0]  = other[0];
	values[1]  = other[1];
	values[2]  = other[2];
	values[3]  = other[3];

	values[4]  = other[4];
	values[5]  = other[5];
	values[6]  = other[6];
	values[7]  = other[7];

	values[8]  = other[8];
	values[9]  = other[9];
	values[10] = other[10];
	values[11] = other[11];

	values[12] = other[12];
	values[13] = other[13];
	values[14] = other[14];
	values[15] = other[15];

	this->isPreMultiply = other.isPreMultiply;
    }

    Mat4& Mat4::identity()
    {
	values[0]  = 1.0;
	values[1]  = 0.0;
	values[2]  = 0.0;
	values[3]  = 0.0;

	values[4]  = 0.0;
	values[5]  = 1.0;
	values[6]  = 0.0;
	values[7]  = 0.0;

	values[8]  = 0.0;
	values[9]  = 0.0;
	values[10] = 1.0;
	values[11] = 0.0;

	values[12] = 0.0;
	values[13] = 0.0;
	values[14] = 0.0;
	values[15] = 1.0;
	return *this;
    }

    Mat4& Mat4::transpose()
    {
	std::swap<float>(values[1],  values[4]);
	std::swap<float>(values[2],  values[8]);
	std::swap<float>(values[3],  values[12]);
	std::swap<float>(values[6],  values[9]);
	std::swap<float>(values[7],  values[13]);
	std::swap<float>(values[11], values[14]);
	return *this;
    }

    Mat4& Mat4::invert()
    {
	float inv[16], det;

	inv[0] = values[5]  * values[10] * values[15] -  values[5]  * values[11] * values[14] - 
	    values[9]  * values[6]  * values[15] +  values[9]  * values[7]  * values[14] +
	    values[13] * values[6]  * values[11] -  values[13] * values[7]  * values[10];

	inv[4] = -values[4]  * values[10] * values[15] + values[4]  * values[11] * values[14] + 
	    values[8]  * values[6]  * values[15] - values[8]  * values[7]  * values[14] - 
	    values[12] * values[6]  * values[11] + values[12] * values[7]  * values[10];

	inv[8] = values[4]  * values[9] * values[15] - values[4]  * values[11] * values[13] - 
	    values[8]  * values[5] * values[15] + values[8]  * values[7] * values[13] + 
	    values[12] * values[5] * values[11] - values[12] * values[7] * values[9];

	inv[12] = -values[4]  * values[9] * values[14] + values[4]  * values[10] * values[13] +
	    values[8]  * values[5] * values[14] - values[8]  * values[6] * values[13] - 
	    values[12] * values[5] * values[10] + values[12] * values[6] * values[9];

	inv[1] = -values[1]  * values[10] * values[15] + values[1]  * values[11] * values[14] + 
	    values[9]  * values[2] * values[15] - values[9]  * values[3] * values[14] - 
	    values[13] * values[2] * values[11] + values[13] * values[3] * values[10];

	inv[5] = values[0]  * values[10] * values[15] - values[0]  * values[11] * values[14] - 
	    values[8]  * values[2] * values[15] + values[8]  * values[3] * values[14] + 
	    values[12] * values[2] * values[11] - values[12] * values[3] * values[10];

	inv[9] = -values[0]  * values[9] * values[15] + values[0]  * values[11] * values[13] + 
	    values[8]  * values[1] * values[15] - values[8]  * values[3] * values[13] - 
	    values[12] * values[1] * values[11] + values[12] * values[3] * values[9];

	inv[13] = values[0]  * values[9] * values[14] - values[0]  * values[10] * values[13] - 
	    values[8]  * values[1] * values[14] + values[8]  * values[2] * values[13] + 
	    values[12] * values[1] * values[10] - values[12] * values[2] * values[9];

	inv[2] = values[1]  * values[6] * values[15] - values[1]  * values[7] * values[14] - 
	    values[5]  * values[2] * values[15] + values[5]  * values[3] * values[14] + 
	    values[13] * values[2] * values[7] - values[13] * values[3] * values[6];

	inv[6] = -values[0]  * values[6] * values[15] + values[0]  * values[7] * values[14] + 
	    values[4]  * values[2] * values[15] - values[4]  * values[3] * values[14] - 
	    values[12] * values[2] * values[7] + values[12] * values[3] * values[6];

	inv[10] = values[0]  * values[5] * values[15] - values[0]  * values[7] * values[13] - 
	    values[4]  * values[1] * values[15] + values[4]  * values[3] * values[13] + 
	    values[12] * values[1] * values[7] - values[12] * values[3] * values[5];

	inv[14] = -values[0]  * values[5] * values[14] + values[0]  * values[6] * values[13] + 
	    values[4]  * values[1] * values[14] - values[4]  * values[2] * values[13] - 
	    values[12] * values[1] * values[6] + values[12] * values[2] * values[5];

	inv[3] = -values[1] * values[6] * values[11] + values[1] * values[7] * values[10] + 
	    values[5] * values[2] * values[11] - values[5] * values[3] * values[10] - 
	    values[9] * values[2] * values[7] + values[9] * values[3] * values[6];

	inv[7] = values[0] * values[6] * values[11] - values[0] * values[7] * values[10] - 
	    values[4] * values[2] * values[11] + values[4] * values[3] * values[10] + 
	    values[8] * values[2] * values[7] - values[8] * values[3] * values[6];

	inv[11] = -values[0] * values[5] * values[11] + values[0] * values[7] * values[9] + 
	    values[4] * values[1] * values[11] - values[4] * values[3] * values[9] - 
	    values[8] * values[1] * values[7] + values[8] * values[3] * values[5];

	inv[15] = values[0] * values[5] * values[10] - values[0] * values[6] * values[9] - 
	    values[4] * values[1] * values[10] + values[4] * values[2] * values[9] + 
	    values[8] * values[1] * values[6] - values[8] * values[2] * values[5];

	det = values[0] * inv[0] + values[1] * inv[4] + values[2] * inv[8] + values[3] * inv[12];

	// if (det == 0)
	// {
	// 	std::cout << "Warnning:: Matrix4x4's inverse is not exsit!" << std::endl;	
	//     return this->identity();
	// }
	if(det <= 1e-6)
	{
	    std::cout << "Warnning:: Matrix4x4's inverse is not exist!" << std::endl;
	    return this->identity();    
	}

	det = 1.0 / det;

	for (int i = 0; i < 16; i++)
	    values[i] = inv[i] * det;

	return *this;
    }

    Mat4& Mat4::translate(float x, float y, float z)
    {
	float mat[16];
	mat[0]  = 1.0;
	mat[1]  = 0.0;
	mat[2]  = 0.0;
	mat[3]  = 0.0;

	mat[4]  = 0.0;
	mat[5]  = 1.0;
	mat[6]  = 0.0;
	mat[7]  = 0.0;

	mat[8]  = 0.0;
	mat[9]  = 0.0;
	mat[10] = 1.0;
	mat[11] = 0.0;

	mat[12] = x;
	mat[13] = y;
	mat[14] = z;
	mat[15] = 1.0;

	// if(isPreMultiply) return mat * (*this);		// Pre-multiplication
	// else return *this * mat;			// Post-multiplication (the same as OpenGL)
	Mat4& ans = isPreMultiply ? (mat * (*this)) : (*this * mat);
	return this->set(ans.get());
    }

    Mat4& Mat4::translate(const Vec3& pos)
    {
	return this->translate(pos.x, pos.y, pos.z);
    }

    Mat4& Mat4::rotate(float angle, float x, float y, float z)
    {
	float s = sinf(angle);
	float c = cosf(angle);
	float m = 1 - c;

	float mat[16];
	mat[0]  = m * x * x + c;
	mat[1]  = m * x * y + s * z;
	mat[2]  = m * x * z - s * y;
	mat[3]  = 0.0;

	mat[4]  = m * x * y - s * z;
	mat[5]  = m * y * y + c;
	mat[6]  = m * y * z + s * x;
	mat[7]  = 0.0;

	mat[8]  = m * x * z + s * y;
	mat[9]  = m * y * z - s * x;
	mat[10] = m * z * z + c;
	mat[11] = 0.0;

	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 0;
	mat[15] = 1.0;

	// if(isPreMultiply) return mat * (*this);	// Pre-multiplication
	// else return *this * mat;			// Post-multiplication (the same as OpenGL)
	Mat4& ans = isPreMultiply ? (mat * (*this)) : (*this * mat);
	return this->set(ans.get());
    }

    Mat4& Mat4::rotate(float angle, const Vec3& axis)
    {
	return this->rotate(angle, axis.x, axis.y, axis.z);
    }

    Mat4& Mat4::scale(float s)
    {
	return this->scale(s, s, s);
    }

    Mat4& Mat4::scale(float x, float y, float z)
    {
	float mat[16];
	mat[0]  = x;
	mat[1]  = 0.0;
	mat[2]  = 0.0;
	mat[3]  = 0.0;

	mat[4]  = 0.0;
	mat[5]  = y;
	mat[6]  = 0.0;
	mat[7]  = 0.0;

	mat[8]  = 0.0;
	mat[9]  = 0.0;
	mat[10] = z;
	mat[11] = 0.0;

	mat[12] = 0.0;
	mat[13] = 0.0;
	mat[14] = 0.0;
	mat[15] = 1.0;

	// if(isPreMultiply) return mat * (*this);		// Pre-multiplication
	// else return *this * mat;			// Post-multiplication (the same as OpenGL)
	Mat4& ans = isPreMultiply ? (mat * (*this)) : (*this * mat);
	return this->set(ans.get());
    }

    Mat4& Mat4::scale(const Vec3& scale)
    {
	return this->scale(scale.x, scale.y, scale.z);
    }

    Mat4& Mat4::lookAt(float px, float py, float pz)
    {
	// get forward vector
	float  tx = px - this->values[12];
	float  ty = py - this->values[13];
	float  tz = pz - this->values[14];
	// float  tx = px + this->values[12];
	// float  ty = py + this->values[13];
	// float  tz = pz + this->values[14];

	// forward vector is tx, ty, tz, and normalize forward vector
	float tm = sqrtf(tx * tx + ty * ty + tz * tz);
	float fx = tx / tm;
	float fy = ty / tm;
	float fz = tz / tm;

	// compute temporal up vector
	// if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
	float ux = 0, uy = 0, uz = 0;
	if(fabs(fx) < 1e-5 && fabs(fz) < 1e-5)
	    if(fy > 0) uz = -1.0;
	    else uz = 1.0;
	else
	    uy = 1.0;

	// compute side vector as:
	// (up x forward) --> [left-handed] 
	// float sx = uy * fz - uz * fy;
	// float sy = uz * fx - ux * fz;
	// float sz = ux * fy - uy * fx;
	// (forward x up) --> [right-handed]
	float sx = fy * uz - fz * uy;
	float sy = fz * ux - fx * uz;
	float sz = fx * uy - fy * ux;

	// normalize side vector
	float sm = sqrtf(sx * sx + sy * sy + sz * sz);
	sx /= sm;
	sy /= sm;
	sz /= sm;

	// recompute up vector as:
	// (forward x side) --> [left-handed]
	// ux = fy * sz - fz * sy;
	// uy = fz * sx - fx * sz;
	// uz = fx * sy - fy * sx;
	// (side x forward) --> [right-handed]
	ux = sy * fz - sz * fy;
	uy = sz * fx - sx * fz;
	uz = sx * fy - sy * fx;

	values[0]  = sx;
	values[4]  = sy;
	values[8]  = sz;

	values[1]  = ux;
	values[5]  = uy;
	values[9]  = uz;

	values[2]  = -fx;
	values[6]  = -fy;
	values[10] = -fz;

	// values[3]  = 0.0;
	// values[7]  = 0.0;
	// values[11] = 0.0;
	// values[15] = 1.0;

	// compute translation part
	// values[12] = -(sx * px + sy * py + sz * pz);	// -dot(side, position)
	// values[13] = -(sx * ux + sy * uy + sz * uz);	// -dot(up, position)
	// values[14] = -(sx * fx + sy * fy + sz * fz);	// -dot(forward, position)
    
	return *this;
    }

    Mat4& Mat4::lookAt(const Vec3& target)
    {
	return this->lookAt(target.x, target.y, target.z);
    }

    Mat4& Mat4::lookAt(float fx, float fy, float fz, float ux, float uy , float uz)
    {
	// get positon vector
	// float px = values[12];
	// float py = values[13];
	// float pz = values[14];

	// compute forward vector
	// float fx = tx - px;
	// float fy = ty - py;
	// float fz = tz - pz;

	// forward vector is tx, ty, tz

	// normalize forward vector
	float fm = sqrtf(fx * fx + fy * fy + fz * fz);
	fx = fx / fm;
	fy = fy / fm;
	fz = fz / fm;

	// compute side vector as:
	// (up x forward) --> [left-handed] 
	// float sx = uy * fz - uz * fy;
	// float sy = uz * fx - ux * fz;
	// float sz = ux * fy - uy * fx;
	// (forward x up) --> [right-handed]
	float sx = fy * uz - fz * uy;
	float sy = fz * ux - fx * uz;
	float sz = fx * uy - fy * ux;

	// normalize side vector
	float sm = sqrtf(sx * sx + sy * sy + sz * sz);
	sx /= sm;
	sy /= sm;
	sz /= sm;

	// recompute up vector as:
	// (forward x side) --> [left-handed]
	// ux = fy * sz - fz * sy;
	// uy = fz * sx - fx * sz;
	// uz = fx * sy - fy * sx;
	// (side x forward) --> [right-handed]
	ux = sy * fz - sz * fy;
	uy = sz * fx - sx * fz;
	uz = sx * fy - sy * fx;

	values[0]  = sx;
	values[4]  = sy;
	values[8]  = sz;

	values[1]  = ux;
	values[5]  = uy;
	values[9]  = uz;

	values[2]  = -fx;
	values[6]  = -fy;
	values[10] = -fz;

	// values[3]  = 0.0;
	// values[7]  = 0.0;
	// values[11] = 0.0;
	// values[15] = 1.0;

	// compute translation part
	// values[12] = -(sx * px + sy * py + sz * pz);	// -dot(side, position)
	// values[13] = -(sx * ux + sy * uy + sz * uz);	// -dot(up, position)
	// values[14] =  (sx * fx + sy * fy + sz * fz);	//  dot(forward, position)
    
	return *this;
    }

    Mat4& Mat4::lookAt(const Vec3& forward, const Vec3& up)
    {
	return this->lookAt(forward.x, forward.y, forward.z, up.x, up.y, up.z);
    }

    Mat4& Mat4::lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy , float uz)
    {
	// get forward vector
	float fx = cx - ex;
	float fy = cy - ey;
	float fz = cz - ez;

	// normalize forward vector
	float fm = sqrtf(fx * fx + fy * fy + fz * fz);
	fx = fx / fm;
	fy = fy / fm;
	fz = fz / fm;

	// compute side vector as:
	// (up x forward) --> [left-handed] 
	// float sx = uy * fz - uz * fy;
	// float sy = uz * fx - ux * fz;
	// float sz = ux * fy - uy * fx;
	// (forward x up) --> [right-handed]
	float sx = fy * uz - fz * uy;
	float sy = fz * ux - fx * uz;
	float sz = fx * uy - fy * ux;

	// normalize side vector
	float sm = sqrtf(sx * sx + sy * sy + sz * sz);
	sx /= sm;
	sy /= sm;
	sz /= sm;

	// recompute up vector as:
	// (forward x side) --> [left-handed]
	// ux = fy * sz - fz * sy;
	// uy = fz * sx - fx * sz;
	// uz = fx * sy - fy * sx;
	// (side x forward) --> [right-handed]
	ux = sy * fz - sz * fy;
	uy = sz * fx - sx * fz;
	uz = sx * fy - sy * fx;

	values[0]  = sx;
	values[4]  = sy;
	values[8]  = sz;

	values[1]  = ux;
	values[5]  = uy;
	values[9]  = uz;

	values[2]  = -fx;
	values[6]  = -fy;
	values[10] = -fz;

	values[3]  = 0.0;
	values[7]  = 0.0;
	values[11] = 0.0;
	values[15] = 1.0;

	// compute translation part
	values[12] = -(ex * sx + ey * sy + ez * sz);	// -dot(side, position)
	values[13] = -(ex * ux + ey * uy + ez * uz);	// -dot(up, position)
	values[14] =  (ex * fx + ey * fy + ez * fz);	//  dot(forward, position) -->[right-handed] or -dot(forward, position) -->[left-handed]
    
	return *this;
    }

    Mat4& Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
	return this->lookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
    }

    Mat4& Mat4::lookAtLH(float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy , float uz)
    {
	// get forward vector
	float fx = cx - ex;
	float fy = cy - ey;
	float fz = cz - ez;

	// normalize forward vector
	float fm = sqrtf(fx * fx + fy * fy + fz * fz);
	fx = fx / fm;
	fy = fy / fm;
	fz = fz / fm;

	// compute side vector as:
	// (up x forward) --> [left-handed] 
	float sx = uy * fz - uz * fy;
	float sy = uz * fx - ux * fz;
	float sz = ux * fy - uy * fx;

	// normalize side vector
	float sm = sqrtf(sx * sx + sy * sy + sz * sz);
	sx /= sm;
	sy /= sm;
	sz /= sm;

	// recompute up vector as:
	// (forward x side) --> [left-handed]
	ux = fy * sz - fz * sy;
	uy = fz * sx - fx * sz;
	uz = fx * sy - fy * sx;

	values[0]  = sx;
	values[4]  = sy;
	values[8]  = sz;

	values[1]  = ux;
	values[5]  = uy;
	values[9]  = uz;

	values[2]  = fx;
	values[6]  = fy;
	values[10] = fz;

	values[3]  = 0.0;
	values[7]  = 0.0;
	values[11] = 0.0;
	values[15] = 1.0;

	// compute translation part
	values[12] = -(ex * sx + ey * sy + ez * sz);	// -dot(side, position)
	values[13] = -(ex * ux + ey * uy + ez * uz);	// -dot(up, position)
	values[14] = -(ex * fx + ey * fy + ez * fz);	// -dot(forward, position) -->[left-handed]
	return *this;
    }

    Mat4& Mat4::lookAtLH(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
	return this->lookAtLH(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
    }

    Mat4& Mat4::frustum(float left, float right, float bottom, float top, float near, float far)
    {
	float dnear  = near * 2.0;
	float width  = right - left;
	float height = top - bottom;
	float depth  = far - near;

	values[0]  = dnear / width;
	values[1]  = 0.0;
	values[2]  = 0.0;
	values[3]  = 0.0;

	values[4]  = 0.0;
	values[5]  = dnear / height;
	values[6]  = 0.0;
	values[7]  = 0.0;

	values[8]  = (right + left) / width;
	values[9]  = (top + bottom) / height;
	values[10] = -(far + near)  / depth;
	values[11] = -1.0;

	values[12] = 0.0;
	values[13] = 0.0;
	values[14] = -1.0 * far * dnear / depth;	//  0 - 1
	// values[14] = -2.0 * far * dnear / depth;	// -1 - 1
	values[15] = 0.0;
	return *this;
    }

    Mat4& Mat4::perspective(float fovy, float aspect, float near, float far)
    {
	float tan = tanf(fovy * 0.5);	// tangent of half fovy
	// float tan = tanf(fovy * 0.5 * 0.01745329251 /*deg2rad*/ );	// tangent of half fovy in radians
	float height = near * tan;
	float width  = height * aspect;
	return this->frustum(-width, width, -height, height, near, far);
    }

    Mat4& Mat4::ortho(float left, float right, float bottom, float top, float near, float far)
    {
	float width  = right - left;
	float height = top - bottom;
	float depth  = far - near;

	values[0]  = 2.0 / width;
	values[1]  = 0.0;
	values[2]  = 0.0;
	values[3]  = 0.0;

	values[4]  = 0.0;
	values[5]  = 2.0 / height;
	values[6]  = 0.0;
	values[7]  = 0.0;

	values[8]  = 0.0;
	values[9]  = 0.0;
	values[10] = -2.0 / depth;
	values[11] = 0.0;

	values[12] = -(right + left) / width;
	values[13] = -(top + bottom) / height;
	values[14] =  -(far + near)  / depth;
	values[15] = 1.0;
	return *this;
    }

    Mat4& Mat4::set(float mat[16])
    {
	values[0]  = mat[0];
	values[1]  = mat[1];
	values[2]  = mat[2];
	values[3]  = mat[3];

	values[4]  = mat[4];
	values[5]  = mat[5];
	values[6]  = mat[6];
	values[7]  = mat[7];

	values[8]  = mat[8];
	values[9]  = mat[9];
	values[10] = mat[10];
	values[11] = mat[11];

	values[12] = mat[12];
	values[13] = mat[13];
	values[14] = mat[14];
	values[15] = mat[15];
	return *this;
    }

    float* Mat4::get()
    {
	return values;
    }

    const float* Mat4::get() const
    {
	return this->values;
    }

    Mat4& Mat4::operator=(const Mat4& other)
    {
	values[0]  = other[0];
	values[1]  = other[1];
	values[2]  = other[2];
	values[3]  = other[3];

	values[4]  = other[4];
	values[5]  = other[5];
	values[6]  = other[6];
	values[7]  = other[7];

	values[8]  = other[8];
	values[9]  = other[9];
	values[10] = other[10];
	values[11] = other[11];

	values[12] = other[12];
	values[13] = other[13];
	values[14] = other[14];
	values[15] = other[15];

	return *this;
    }

    float Mat4::operator[](int idx)
    {
	assert(idx > -1 && idx < 16);
	return values[idx];
    }

    float Mat4::operator[](int idx) const
    {
	assert(idx > -1 && idx < 16);
	return values[idx];
    }

    //
    // Free funtion
    //
    Mat4& operator*(const Mat4&lhs, const Mat4& rhs)
    {
	float mat[16];
	mat[0]  = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8]  * rhs[2] + lhs[12] * rhs[3];
	mat[1]  = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9]  * rhs[2] + lhs[13] * rhs[3];
	mat[2]  = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
	mat[3]  = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

	mat[4]  = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8]  * rhs[6] + lhs[12] * rhs[7];
	mat[5]  = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9]  * rhs[6] + lhs[13] * rhs[7];
	mat[6]  = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
	mat[7]  = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

	mat[8]  = lhs[0] * rhs[8] + lhs[4] * rhs[9] +  lhs[8]  * rhs[10] + lhs[12] * rhs[11];
	mat[9]  = lhs[1] * rhs[8] + lhs[5] * rhs[9] +  lhs[9]  * rhs[10] + lhs[13] * rhs[11];
	mat[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] +  lhs[10] * rhs[10] + lhs[14] * rhs[11];
	mat[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] +  lhs[11] * rhs[10] + lhs[15] * rhs[11];

	mat[12] =  lhs[0]* rhs[12] + lhs[4] * rhs[13] + rhs[14] * lhs[8]  + lhs[12] * rhs[15];
	mat[13] =  lhs[1]* rhs[12] + lhs[5] * rhs[13] + rhs[14] * lhs[9]  + lhs[13] * rhs[15];
	mat[14] =  lhs[2]* rhs[12] + lhs[6] * rhs[13] + rhs[14] * lhs[10] + lhs[14] * rhs[15];
	mat[15] =  lhs[3]* rhs[12] + lhs[7] * rhs[13] + rhs[14] * lhs[11] + lhs[15] * rhs[15];
	return (new Mat4())->set(mat);
    }

    Mat4& operator*(Mat4& lhs, float rhs[16])
    {
	float mat[16];
	mat[0]  = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8]  * rhs[2] + lhs[12] * rhs[3];
	mat[1]  = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9]  * rhs[2] + lhs[13] * rhs[3];
	mat[2]  = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
	mat[3]  = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

	mat[4]  = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8]  * rhs[6] + lhs[12] * rhs[7];
	mat[5]  = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9]  * rhs[6] + lhs[13] * rhs[7];
	mat[6]  = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
	mat[7]  = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

	mat[8]  = lhs[0] * rhs[8] + lhs[4] * rhs[9] +  lhs[8]  * rhs[10] + lhs[12] * rhs[11];
	mat[9]  = lhs[1] * rhs[8] + lhs[5] * rhs[9] +  lhs[9]  * rhs[10] + lhs[13] * rhs[11];
	mat[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] +  lhs[10] * rhs[10] + lhs[14] * rhs[11];
	mat[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] +  lhs[11] * rhs[10] + lhs[15] * rhs[11];

	mat[12] =  lhs[0]* rhs[12] + lhs[4] * rhs[13] + rhs[14] * lhs[8]  + lhs[12] * rhs[15];
	mat[13] =  lhs[1]* rhs[12] + lhs[5] * rhs[13] + rhs[14] * lhs[9]  + lhs[13] * rhs[15];
	mat[14] =  lhs[2]* rhs[12] + lhs[6] * rhs[13] + rhs[14] * lhs[10] + lhs[14] * rhs[15];
	mat[15] =  lhs[3]* rhs[12] + lhs[7] * rhs[13] + rhs[14] * lhs[11] + lhs[15] * rhs[15];
	// return lhs.set(mat);
	return (new Mat4())->set(mat);
    }

    Mat4& operator*(float lhs[16], Mat4& rhs)
    {
	float mat[16];
	mat[0]  = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8]  * rhs[2] + lhs[12] * rhs[3];
	mat[1]  = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9]  * rhs[2] + lhs[13] * rhs[3];
	mat[2]  = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
	mat[3]  = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

	mat[4]  = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8]  * rhs[6] + lhs[12] * rhs[7];
	mat[5]  = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9]  * rhs[6] + lhs[13] * rhs[7];
	mat[6]  = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
	mat[7]  = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

	mat[8]  = lhs[0] * rhs[8] + lhs[4] * rhs[9] +  lhs[8]  * rhs[10] + lhs[12] * rhs[11];
	mat[9]  = lhs[1] * rhs[8] + lhs[5] * rhs[9] +  lhs[9]  * rhs[10] + lhs[13] * rhs[11];
	mat[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] +  lhs[10] * rhs[10] + lhs[14] * rhs[11];
	mat[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] +  lhs[11] * rhs[10] + lhs[15] * rhs[11];

	mat[12] =  lhs[0]* rhs[12] + lhs[4] * rhs[13] + rhs[14] * lhs[8]  + lhs[12] * rhs[15];
	mat[13] =  lhs[1]* rhs[12] + lhs[5] * rhs[13] + rhs[14] * lhs[9]  + lhs[13] * rhs[15];
	mat[14] =  lhs[2]* rhs[12] + lhs[6] * rhs[13] + rhs[14] * lhs[10] + lhs[14] * rhs[15];
	mat[15] =  lhs[3]* rhs[12] + lhs[7] * rhs[13] + rhs[14] * lhs[11] + lhs[15] * rhs[15];
	// return rhs.set(mat);
	return (new Mat4())->set(mat);
    }

    std::ostream& operator<<(std::ostream& os, const Mat4& rhs)
    {
	int w = 10;
	os << std::fixed << std::setprecision(5);
	os << "[ " << rhs[0] << " " << std::setw(w) << rhs[4] << " " << std::setw(w) << rhs[8]  <<  " " << std::setw(w) << rhs[12] << " ]\n"
	   << "[ " << rhs[1] << " " << std::setw(w) << rhs[5] << " " << std::setw(w) << rhs[9]  <<  " " << std::setw(w) << rhs[13] << " ]\n"
	   << "[ " << rhs[2] << " " << std::setw(w) << rhs[6] << " " << std::setw(w) << rhs[w]  <<  " " << std::setw(w) << rhs[14] << " ]\n"
	   << "[ " << rhs[3] << " " << std::setw(w) << rhs[7] << " " << std::setw(w) << rhs[11] <<  " " << std::setw(w) << rhs[15] << " ]\n";
	os << std::setiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
    }

    
}
