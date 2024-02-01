#ifndef TEXTURE_H
#define TEXTURE_H
#include "Graphics.h"

namespace BINDU
{
	class Texture 
	{
	private:

		ComPtr<ID2D1Bitmap>		m_bitmap;

	protected:

		friend class Sprite;

	public:
		Texture() = default;

		Texture(const char* filename)
		{
			LoadFromFile(filename);
		}

		~Texture() = default;

		void	LoadFromFile(const char* filename);

		ID2D1Bitmap* getBitmap() const { return m_bitmap.Get(); }

	};

};

#endif // TEXTURE_H
