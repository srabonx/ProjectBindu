#include "Bindu.h"
#pragma once


namespace BINDU
{
	class SpriteBatch
	{
	private:
		ComPtr<ID2D1SpriteBatch> m_spriteBatch;
		ComPtr<ID2D1DeviceContext3> m_deviceContxt;
		ComPtr<ID2D1Bitmap> m_bitmap;
		D2D1_RECT_F rect;

	public:
		SpriteBatch();
		~SpriteBatch();
		HRESULT Init();
		bool LoadFromFile(const wchar_t* filename);
		void DrawSpriteBatch();
		// Accessor/Mutator functions
		inline ID2D1SpriteBatch* getSpriteBatch() { return m_spriteBatch.Get(); }

	};
}// namespace