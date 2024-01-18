
#include "Include/Bindu.h"

namespace BINDU
{
	bool Drawable::LoadFromFile(const wchar_t* filename)
	{
		if (!BitmapLoader::LoadFromFile(filename, m_bitmap.ReleaseAndGetAddressOf()))
		{
			g_engine->sendMessage("Error loading bitmap!", "Error", e_Error::FATAL_ERROR);
			return false;
		}

		const D2D1_SIZE_F lSize = m_bitmap->GetSize();
		m_bitmapSize = { static_cast<int>(lSize.width),static_cast <int> (lSize.height) };

		return true;
	}

	void Drawable::SetBitmap(ID2D1Bitmap* bitmap)
	{
		m_bitmap = bitmap;
		const D2D1_SIZE_F lSize = m_bitmap->GetSize();
		m_bitmapSize = { static_cast<int>(lSize.width),static_cast <int> (lSize.height) };
	}

	void Drawable::setDstRect(const Bnd_Rect_F& dstRect)
	{
		
	}

	Bnd_Rect_F Drawable::getDstRect() const
	{


		return {};
	}

	void Drawable::setSrcRect(const Bnd_Rect_F* srcRect)
	{

	}

	Bnd_Rect_F Drawable::getSrcRect() const
	{
		return {};
	}
};