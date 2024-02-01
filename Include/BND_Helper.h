#ifndef BND_HELPER_H
#define BND_HELPER_H
#include <d2d1.h>

namespace BINDU
{
	struct Bnd_Rect
	{
		Bnd_Rect() = default;
		Bnd_Rect(const int x,const int y,const int w,const int h) : x(x), y(y), w(w), h(h) {}
		unsigned int x{ 0 };
		unsigned int y{ 0 };
		unsigned int w{ 0 };
		unsigned int h{ 0 };

		D2D1_RECT_U operator()() const
		{
			return D2D1::RectU( x,y,x + w,h + y );
		}

	};

	struct Bnd_Rect_F
	{
		Bnd_Rect_F() = default;
		Bnd_Rect_F(const float x, const float y, const float w, const float h) : x(x), y(y), w(w), h(h) {}
		float x{ 0 };
		float y{ 0 };
		float w{ 0 };
		float h{ 0 };

		D2D1_RECT_F operator()() const
		{
			return D2D1::RectF( x,y,x + w,h + y );
		}

	};

	struct Bnd_Size
	{
		Bnd_Size() = default;
		Bnd_Size(const int width, const int height) :width(width), height(height) {};
		int width{ 1 };
		int height{ 1 };
	};

	struct Bnd_Size_F
	{
		Bnd_Size_F() = default;
		Bnd_Size_F(const float width, const float height) :width(width), height(height) {};
		float width{ 1 };
		float height{ 1 };
	};

};

#endif // BND_HELPER_H
