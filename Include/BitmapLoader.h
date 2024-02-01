#pragma once
#include <d2d1.h>

namespace BINDU
{

	class BitmapLoader
	{

	public:
		BitmapLoader();
		~BitmapLoader();

		static bool LoadFromFile(const wchar_t* filename, ID2D1Bitmap** ppBitmap);
	};// Class

};// Namespace

