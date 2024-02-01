#include "Include/Texture.h"

#include "Include/BitmapLoader.h"
#include "Include/StrConverter.h"

namespace BINDU
{
	void Texture::LoadFromFile(const char* filename)
	{
		BitmapLoader::LoadFromFile(StrConverter::getWideStr(filename).c_str(), m_bitmap.ReleaseAndGetAddressOf());
	}

};


