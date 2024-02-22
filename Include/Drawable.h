#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <wrl/client.h>
#include "BND_Helper.h"
#include "Graphics.h"
#include "Vector.h"

using Microsoft::WRL::ComPtr;


namespace BINDU
{

	class Drawable 
	{
	public:

		              Drawable() = default;
		virtual		 ~Drawable() = default;

	protected:

		ComPtr<ID2D1Bitmap>		m_bitmap;
		Bnd_Size_F				m_bitmapSize;

		virtual void  Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) = 0;

		void		  SetBitmap(ID2D1Bitmap* bitmap);

		inline	Bnd_Size_F  getBitmapSize() const { return m_bitmapSize; }


	};// class
};


#endif // DRAWABLE_H
