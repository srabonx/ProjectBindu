#include "Include/Bindu.h"

namespace BINDU
{
	SpriteBatch::SpriteBatch()
	{
		m_spriteBatch = nullptr;
		m_deviceContxt = nullptr;
		m_bitmap = nullptr;
		rect = { 0,0,50,50 };
	}

	SpriteBatch::~SpriteBatch()
	{
		
	}

	HRESULT SpriteBatch::Init()
	{
		HRESULT hr = S_OK;

// 		DX::ThrowIfFailed(
// 
// 			g_engine->getGraphics()->getD2dDeviceContext()->QueryInterface(m_deviceContxt.ReleaseAndGetAddressOf())
// 		
// 		);

// 		DX::ThrowIfFailed(
// 
// 			m_deviceContxt->CreateSpriteBatch(m_spriteBatch.ReleaseAndGetAddressOf())
// 
// 		);

		DX::ThrowIfFailed(
			g_engine->getGraphics()->getD2dDeviceContext()->CreateSpriteBatch(m_spriteBatch.ReleaseAndGetAddressOf())
		);
		m_spriteBatch->AddSprites(20, &rect, NULL, NULL);
		return hr;
	}
	bool SpriteBatch::LoadFromFile(const wchar_t* filename)
	{
		
		if (!BitmapLoader::LoadFromFile(filename, &m_bitmap))
			return false;

		if (!m_bitmap)
			return false;
		return true;
	}
	void SpriteBatch::DrawSpriteBatch()
	{
		g_engine->getGraphics()->getD2dDeviceContext()->DrawSpriteBatch(m_spriteBatch.Get(), m_bitmap.Get());
	}
};// namespace