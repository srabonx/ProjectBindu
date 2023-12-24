#include "Bindu.h"
#pragma once

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

