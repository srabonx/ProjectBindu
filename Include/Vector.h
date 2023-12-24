#include "Bindu.h"
#pragma once

namespace BINDU {

	template<class T>
	struct Vector2 
	{
		T x;																// Variable X
		T y;																// Variable Y

		Vector2() :x(0), y(0) {}											// Default constructor
		Vector2(T x, T y) :x(x), y(y) {}									// Parameterize constructor
		Vector2(const Vector2& vec2) :x(vec2.x), y(vec2.y) {}				// Copy constructor

		inline void setX(T x) { this->x = x; }
		inline T getX() const { return x; }

		inline void setY(T y) { this->y = y; }
		inline T getY() const { return y; }

		inline Vector2 operator + (const Vector2& rhs)
		{
			T nx = x + rhs.x;
			T ny = y + rhs.y;
			return Vector2(nx, ny);
		}

		inline void operator += (const Vector2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
		}

		inline operator Vector2<int>() const
		{
			return { static_cast<int>(this->x),static_cast<int>(this->y) };
		}


		inline operator Vector2<float>() const
		{
			return { static_cast<float>(this->x),static_cast<float>(this->y) };
		}

		inline operator T() const {
			return { static_cast<T>(x),static_cast<T>(y) };
		}

	}; // class

	typedef Vector2<int> Vec2i;
	typedef Vector2<float> Vec2f;

};// namespace
