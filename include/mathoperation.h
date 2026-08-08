#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

namespace Mmt
{
	class Mathmethod
	{
	public:
		struct Vec2
		{
			float x;
			float y;

			Vec2() : x(0.0f), y(0.0f) {}
			Vec2(float _x, float _y) : x(_x), y(_y) {}
			float Length() const
			{
				return sqrt(x * x + y * y);
			}
			float Dot(const Vec2& other) const
			{
				return x * other.x + y * other.y;
			}
			float Cross(const Vec2& other) const
			{
				return x * other.y - y * other.x;
			}
			float Distance(const Vec2& other) const
			{
				return sqrt(abs((other.x - x) * (other.x - x)) + abs((other.y - y) * (other.y - y)));
			}
			Vec2 Normalize() const
			{
				float len = Length();
				return Vec2(x / len, y / len);
			}
			Vec2 Lerp(const Vec2& start, const Vec2& end, float t) const
			{
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;
				return start + (end - start) * t;
			}
			Vec2 operator+(const Vec2& other) const
			{
				return Vec2
				(
					x + other.x,
					y + other.y
				);
			}
			Vec2 operator-(const Vec2& other) const
			{
				return Vec2
				(
					x - other.x,
					y - other.y
				);
			}
			Vec2 operator*(const float real) const
			{
				return Vec2
				(
					x * real,
					y * real
				);
			}
			Vec2 operator/(const float real) const
			{
				return Vec2
				(
					x / real,
					y / real
				);
			}
			Vec2 operator+=(const Vec2& other) 
			{
				return Vec2
				(
					x += other.x,
					y += other.y
				);
			}
			Vec2 operator-=(const Vec2& other) 
			{
				return Vec2
				(
					x -= other.x,
					y -= other.y
				);
			}
			Vec2 operator*=(const Vec2& other) 
			{
				return Vec2
				(
					x *= other.x,
					y *= other.y
				);
			}
			bool operator==(const Vec2& other) const
			{
				const float epsilon = 0.00001f;
				return (abs(x == other.x) < epsilon) && (abs(y == other.y) < epsilon);
			}
			bool operator!=(const Vec2& other) const
			{
				return !(*this == other);
			}
		};
		struct Vec3
		{
			float x;
			float y;
			float z;

			Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
			Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
			float Length() const
			{
				return sqrt(x * x + y * y + z * z);
			}
			float Dot(const Vec3& other) const
			{
				return x * other.x + y * other.y + z * other.z;
			}
			float Distance(const Vec3& other) const
			{
				return sqrt(abs((other.x - x) * (other.x - x)) + abs((other.y - y) * (other.y - y)) + abs((other.z-z)*(other.z-z)));
			}
			Vec3 Normalize() const
			{
				float len = Length();
				return Vec3(x / len, y / len, z / len);
			}
			Vec3 Cross(const Vec3& other) const
			{
				return Vec3(y * other.z - z * other.y, z* other.x - x * other.z, x* other.y - y * other.x);
			}
			Vec3 Lerp(const Vec3& start, const Vec3& end, float t) const
			{
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;
				return start + (end - start) * t;
			}
			Vec3 operator+(const Vec3& other) const
			{
				return Vec3
				(
					x + other.x,
					y + other.y,
					z + other.z
				);
			}
			Vec3 operator-(const Vec3& other) const
			{
				return Vec3
				(
					x - other.x,
					y - other.y,
					z - other.z
				);
			}
			Vec3 operator*(const float real) const
			{
				return Vec3
				(
					x * real,
					y * real,
					z * real
				);
			}
			Vec3 operator/(const float real) const
			{
				return Vec3
				(
					x / real,
					y / real,
					z / real
				);
			}
			Vec3 operator+=(const Vec3& other) 
			{
				return Vec3
				(
					x += other.x,
					y += other.y,
					z += other.z
				);
			}
			Vec3 operator-=(const Vec3& other) 
			{
				return Vec3
				(
					x -= other.x,
					y -= other.y,
					z -= other.z
				);
			}
			Vec3 operator*=(const Vec3& other)
			{
				return Vec3
				(
					x *= other.x,
					y *= other.y,
					z *= other.z
				);
			}
			bool operator==(const Vec3& other) const
			{
				const float epsilon = 0.00001f;
				return (abs(x == other.x) < epsilon) && (abs(y == other.y) < epsilon) && (abs(z == other.z) < epsilon);
			}
			bool operator!=(const Vec3& other) const
			{
				return !(*this == other);
			}
		};
		struct Vec4 // In Homogeneous Coordinates
		{
			float x;
			float y;
			float z;
			float w;

			Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
			Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
			float Length() const
			{
				return sqrt(x * x + y * y + z * z);
			}
			float Dot(const Vec4& other) const
			{
				return x * other.x + y * other.y + z * other.z;
			}
			float Distance(const Vec4& other) const
			{
				return sqrt(abs((other.x - x) * (other.x - x)) + abs((other.y - y) * (other.y - y)) + abs((other.z - z) * (other.z - z)));
			}
			Vec4 Normalize() const
			{
				float len = Length();
				return Vec4(x / len, y / len, z / len,w);
			}
			Vec4 Cross(const Vec4& other) const 
			{
				return Vec4(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, 0.0f);
			}
			Vec4 Lerp(const Vec4& start, const Vec4& end, float t) const
			{
				if (t < 0.0f) t = 0.0f;
				if (t > 1.0f) t = 1.0f;
				return start + (end - start) * t;
			}
			Vec4 operator+(const Vec4& other) const
			{
				return Vec4(
					x + other.x,
					y + other.y,
					z + other.z,
					w + other.w
				);
			}
			Vec4 operator-(const Vec4& other) const
			{
				return Vec4
				(
					x - other.x,
					y - other.y,
					z - other.z,
					w - other.w
				);
			}
			Vec4 operator*(const float real) const
			{
				return Vec4
				(
					x * real,
					y * real,
					z * real,
					w * real
				);
			}
			Vec4 operator/(const float real) const
			{
				return Vec4
				(
					x / real,
					y / real,
					z / real,
					w / real
				);
			}
			Vec4 operator+=(const Vec4& other)
			{
				return Vec4
				(
					x += other.x,
					y += other.y,
					z += other.z,
					w += other.w
				);
			}
			Vec4 operator-=(const Vec4& other)
			{
				return Vec4
				(
					x -= other.x,
					y -= other.y,
					z -= other.z,
					w -= other.w
				);
			}
			Vec4 operator*=(const Vec4& other) 
			{
				return Vec4
				(
					x *= other.x,
					y *= other.y,
					z *= other.z,
					w *= other.w
				);
			}
			bool operator==(const Vec4& other) const
			{
				const float epsilon = 0.00001f;
				return (abs(x == other.x) < epsilon) && (abs(y == other.y) < epsilon) && (abs(z == other.z) < epsilon) && (abs(w == other.w) < epsilon);
			}
			bool operator!=(const Vec4& other) const
			{
				return !(*this == other);
			}
		};
		struct Mat3
		{
			Vec3 col[3];
			Mat3()
			{
				col[0] = Vec3(1.0f, 0.0f, 0.0f);
				col[1] = Vec3(0.0f, 1.0f, 0.0f);
				col[2] = Vec3(0.0f, 0.0f, 1.0f);
			}
			float operator*(const float f) 
			{
				col[0] = Vec3(col[0].x * f, col[0].y * f, col[0].z * f);
				col[1] = Vec3(col[1].x * f, col[1].y * f, col[1].z * f);
				col[2] = Vec3(col[2].x * f, col[2].y * f, col[2].z * f);
			}
			Vec3 operator*(const Vec3& vec) const
			{
				return col[0] * vec.x + col[1] * vec.y + col[2] * vec.z;
			}
			Mat3 operator*(const Mat3& other) const
			{
				Mat3 result;
				result.col[0] = *this * other.col[0];
				result.col[1] = *this * other.col[1];
				result.col[2] = *this * other.col[2];
				return result;
			}
			Mat3 operator+(const Mat3& other) const
			{
				Mat3 result;
				result.col[0] = col[0] + other.col[0];
				result.col[1] = col[1] + other.col[1];
				result.col[2] = col[2] + other.col[2];
				return result;
			}
			Mat3 operator-(const Mat3& other) const
			{
				Mat3 result;
				result.col[0] = col[0] - other.col[0];
				result.col[1] = col[1] - other.col[1];
				result.col[2] = col[2] - other.col[2];
				return result;
			}
			Mat3 operator+=(const Mat3& other) 
			{
				Mat3 result;
				result.col[0] = col[0] += other.col[0];
				result.col[1] = col[1] += other.col[1];
				result.col[2] = col[2] += other.col[2];
				return result;
			}
			Mat3 operator-=(const Mat3& other)
			{
				Mat3 result;
				result.col[0] = col[0] -= other.col[0];
				result.col[1] = col[1] -= other.col[1];
				result.col[2] = col[2] -= other.col[2];
				return result;
			}
			Mat3 Transpose()
			{
				Vec3 temp1 = Vec3(col[0].x,col[1].x,col[2].x);
				Vec3 temp2 = Vec3(col[0].y, col[1].y, col[2].y);
				Vec3 temp3 = Vec3(col[0].z, col[1].z, col[2].z);
				Mat3 result;
				result.col[0] = temp1;
				result.col[1] = temp2;
				result.col[2] = temp3;
				return result;
			}
			Mat3 Inverse()
			{
				float det1 = col[0].x * (col[1].y * col[2].z - col[2].y * col[1].z);
				float det2 = col[1].x * (col[0].y * col[2].z - col[2].y * col[0].z);
				float det3 = col[2].x * (col[0].y * col[1].z - col[1].y * col[0].z);
				float det = det1 - det2 + det3;
				if (det == 0) det = 0.01f;
				Mat3 cofactor;
				cofactor.col[0].x = (col[1].y * col[2].z) - (col[2].y * col[1].z);
				cofactor.col[0].y = -(col[1].x * col[2].z) + (col[2].x * col[1].z);
				cofactor.col[0].z = (col[1].x * col[2].y) - (col[2].x * col[1].y);
				cofactor.col[1].x = -(col[0].y * col[2].z) + (col[2].y * col[0].z);
				cofactor.col[1].y = (col[0].x * col[2].z) - (col[2].x * col[1].z);
				cofactor.col[1].z = -(col[0].x * col[2].y) + (col[2].x * col[0].y);
				cofactor.col[2].x = (col[0].y * col[1].z) - (col[1].y * col[0].z);
				cofactor.col[2].y = -(col[0].x * col[1].z) + (col[1].x * col[0].z);
				cofactor.col[2].z = (col[0].x * col[1].y) - (col[1].x * col[0].y);
				cofactor = cofactor.Transpose();
				Mat3 inverse;
				float div = 1.0f / det;
				inverse.col[0].x = div * cofactor.col[0].x;
				inverse.col[0].y = div * cofactor.col[0].y;
				inverse.col[0].z = div * cofactor.col[0].z;
				inverse.col[1].x = div * cofactor.col[1].x;
				inverse.col[1].y = div * cofactor.col[1].y;
				inverse.col[1].z = div * cofactor.col[1].z;
				inverse.col[2].x = div * cofactor.col[2].x;
				inverse.col[2].y = div * cofactor.col[2].y;
				inverse.col[2].z = div * cofactor.col[2].z;
				return inverse;
			}
		};
		struct Mat4
		{
			Vec4 col[4];
			Mat4()
			{
				col[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
				col[1] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
				col[2] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
				col[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
			Vec4 operator*(const Vec4& vec) const
			{
				return col[0] * vec.x + col[1] * vec.y + col[2] * vec.z + col[3] * vec.w;
			}
			Mat4 operator*(const Mat4& other) const
			{
				Mat4 result;
				result.col[0] = *this * other.col[0];
				result.col[1] = *this * other.col[1];
				result.col[2] = *this * other.col[2];
				result.col[3] = *this * other.col[3];
				return result;
			}
			Mat4 operator+(const Mat4& other) const
			{
				Mat4 result;
				result.col[0] = col[0] + other.col[0];
				result.col[1] = col[1] + other.col[1];
				result.col[2] = col[2] + other.col[2];
				result.col[3] = col[3] + other.col[3];
				return result;
			}
			Mat4 operator-(const Mat4& other) const
			{
				Mat4 result;
				result.col[0] = col[0] - other.col[0];
				result.col[1] = col[1] - other.col[1];
				result.col[2] = col[2] - other.col[2];
				result.col[3] = col[3] - other.col[3];
				return result;
			}
			Mat4 operator+=(const Mat4& other)
			{
				Mat4 result;
				result.col[0] = col[0] += other.col[0];
				result.col[1] = col[1] += other.col[1];
				result.col[2] = col[2] += other.col[2];
				result.col[3] = col[3] += other.col[3];
				return result;
			}
			Mat4 operator-=(const Mat4& other)
			{
				Mat4 result;
				result.col[0] = col[0] -= other.col[0];
				result.col[1] = col[1] -= other.col[1];
				result.col[2] = col[2] -= other.col[2];
				result.col[3] = col[3] -= other.col[3];
				return result;
			}
			Mat4 Translate(const Vec3& t)
			{
				Mat4 result;
				result.col[3] = Vec4(t.x, t.y, t.z, 1.0f);
				return result;
			}
			Mat4 Scale(const Vec3& s)
			{
				Mat4 result;
				result.col[0].x = s.x;
				result.col[1].y = s.y;
				result.col[2].z = s.z;
				return result;
			}
			Mat4 Rotate(float angle, Vec3 axis)
			{
				Mat4 result;
				axis = axis.Normalize();
				float c = cos(angle);
				float s = sin(angle);
				float t = 1.0f - c;
				float x = axis.x;
				float y = axis.y;
				float z = axis.z;
				result.col[0] = Vec4(t * x * x + c, t * x * y - z * s, t * x * z + y * s, 0.0f);
				result.col[1] = Vec4(t * x * y + z * s, t * y * y + c, t * y * z - x * s, 0.0f);
				result.col[2] = Vec4(t * x * z - y * s, t * y * z + x * s, t * z * z + c, 0.0f);
				result.col[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
				return result;
			}
			Mat4 Perspective(float radian, float aspect, float near, float far)
			{
				float f = 1.0f / tan(radian / 2.0f);
				Mat4 result;
				result.col[0] = Vec4(f / aspect, 0.0f, 0.0f, 0.0f);
				result.col[1] = Vec4(0.0f, f, 0.0f, 0.0f);
				result.col[2] = Vec4(0.0f, 0.0f, -(far + near) / (far - near), -1.0f);
				result.col[3] = Vec4(0.0f, 0.0f, -(2.0f*far * near) / (far - near), 0.0f);
				return result;
			}
			Mat4 Lookat(const Vec3& eye, const Vec3& target, const Vec3& up)
			{
				Vec3 zAxis = (eye - target);
				zAxis.Normalize();
				Vec3 xAxis = up.Cross(zAxis).Normalize();
				Vec3 yAxis = zAxis.Cross(xAxis);
				Mat4 result;
				result.col[0] = Vec4(xAxis.x, yAxis.x, zAxis.x, 0.0f);
				result.col[1] = Vec4(xAxis.y, yAxis.y, zAxis.y, 0.0f);
				result.col[2] = Vec4(xAxis.z, yAxis.z, zAxis.z, 0.0f);
				result.col[3] = Vec4(-xAxis.Dot(eye), -yAxis.Dot(eye), -zAxis.Dot(eye), 1.0f);
				return result;
			}
			Mat4 OrthoGraphic(float left, float right, float bottom, float top, float zNear, float zFar)
			{
				Mat4 result;
				result.col[0] = Vec4(2 / (right - left), 0, 0, 0);
				result.col[1] = Vec4(0, 2 / (top - bottom), 0, 0);
				result.col[2] = Vec4(0, 0, -2 / (zFar - zNear), 0);
				result.col[3] = Vec4((-right - left) / (right - left), (-top - bottom) / (top - bottom), (-zFar - zNear) / (zFar - zNear), 1);
				return result;
			}
			Mat3 toMat3()
			{
				Mat3 result;
				result.col[0] = Vec3(col[0].x, col[0].y, col[0].z);
				result.col[1] = Vec3(col[1].x, col[1].y, col[1].z);
				result.col[2] = Vec3(col[2].x, col[2].y, col[2].z);
				return result;
			}
		};
	};
}
