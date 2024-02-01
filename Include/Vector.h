#pragma once
#include "constants.h"

namespace BINDU {

	template<class T>
	struct Vector2 
	{
		T x;																// Variable X
		T y;																// Variable Y

		Vector2() :x(0), y(0) {}											// Default constructor
		Vector2(T x, T y) :x(x), y(y) {}									// Parameterized constructor
		Vector2(const Vector2& vec2) :x(vec2.x), y(vec2.y) {}				// Copy constructor

		inline void setX(T x) { this->x = x; }
		inline T getX() const { return x; }

		inline void setY(T y) { this->y = y; }
		inline T getY() const { return y; }

		inline float Distance(const Vector2& vec)
		{
			return sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y));
		}

		inline float Length()
		{
			return sqrt(x * x + y * y);
		}

		inline Vector2& Normalize()
		{
			(*this) /= Length();

			return (*this);
		}

		inline Vector2& Normal()
		{
			return ((*this) / Length());
		}

		inline T Dot(const Vector2& vec2)
		{
			return ((x * vec2.x) + (y * vec2.y));
		}

		inline float Angle(const Vector2& vec2)
		{
			float degree = atan2(y - vec2.y, x - vec2.x);
			degree = (degree >= 0 ? degree : (2 * PI + degree)) * 180.f / PI;
			return degree;
		}

		inline Vector2 operator + (const Vector2& rhs)
		{
			return Vector2(x + rhs.x, y + rhs.y);
		}

		inline Vector2& operator += (const Vector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;

			return (*this);
		}

		inline Vector2 operator += (const float rhs)
		{
			x += rhs;
			y += rhs;

			return(*this);
		}

		inline Vector2 operator - ()
		{
			return Vector2(-x, -y);
		}

		inline Vector2 operator - (const Vector2& rhs)
		{
			return Vector2(x - rhs.x, y - rhs.y);
		}

		inline Vector2& operator -= (const Vector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;

			return (*this);
		}

		inline Vector2 operator * (const Vector2& rhs)
		{
			return Vector2(x * rhs.x, y * rhs.y);
		}

		inline Vector2& operator *= (const T rhs)
		{
			x *= rhs;
			y *= rhs;

			return (*this);
		}

		inline Vector2 operator / (const T rhs)
		{
			rhs = 1.0f / rhs;

			return Vector2(x * rhs, y * rhs);
		}

		inline Vector2& operator /= (const T rhs)
		{
			rhs = 1.0f / rhs;

			x *= rhs;
			y *= rhs;

			return (*this);
		}


		inline T& operator [](int i)
		{
			return ((&x)[i]);
		}

		inline operator Vector2<int>() const
		{
			return { static_cast<int>(this->x),static_cast<int>(this->y) };
		}


		inline operator Vector2<float>() const
		{
			return { static_cast<float>(this->x),static_cast<float>(this->y) };
		}

		inline Vector2 operator () (T) const
		{
			return { static_cast<T>(x),static_cast<T>(y) };
		}

	}; // class

	typedef Vector2<int> Vec2i;
	typedef Vector2<float> Vec2f;

};// namespace
