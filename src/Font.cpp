
#include "Include/Bindu.h"

namespace BINDU
{
	Font::Font()
	{
	}

	void Font::Init()
	{
		m_spriteBatch.Init();
		m_totalColumn = 16;
	}

	void Font::PrintText(int x, int y, const std::string& text, const BND_COLOR& color,float scale)
	{
		const float fx = static_cast<float> (x);
		const float fy = static_cast<float> (y);

		for( unsigned int i = 0 ; i < text.size(); i++)
		{
			const int frame = static_cast<unsigned char>(text[i]);

			Bnd_Rect_F dstRect{};

			dstRect.x = fx + ( m_charSize.width * scale) * i;
			dstRect.y = fy * scale;
			dstRect.w = m_charSize.width;
			dstRect.h = m_charSize.height;

			Bnd_Rect srcRect = {};

			const int nx = (frame % m_totalColumn) * 9;
			const int ny = (frame/m_totalColumn) * 16;
			

			srcRect.x = nx ;
			srcRect.y = ny ;
			srcRect.w = 9;//static_cast<unsigned int>(m_charSize.width);
			srcRect.h = 16;//static_cast<unsigned int>(m_charSize.height);

			m_spriteBatch.AddSprite(dstRect, &srcRect, color,scale);
			
		}
		m_spriteBatch.Draw(nullptr,D2D1::Matrix3x2F::Identity());
		m_spriteBatch.Clear();
	}

	bool Font::LoadWidthData(const char* filename)
	{
		return true;
	}

	bool Font::LoadBitmapFont(const wchar_t* filename)
	{

		m_spriteBatch.LoadSpriteSheet(filename);
		return true;
	}
};