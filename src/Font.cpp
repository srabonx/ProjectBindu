
#include "Include/Font.h"

#include <fstream>

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

//		m_spriteBatch.Clear();

		for( unsigned int i = 0 ; i < text.size(); i++)
		{
			const int frame = static_cast<unsigned char>(text[i]);

			Bnd_Rect_F dstRect{};

			if (m_widths[frame] == 0) m_widths[frame] = static_cast<int>(m_charSize.width);

			dstRect.x = fx + (static_cast<float>(m_widths[frame]) * scale) * i;
			dstRect.y = fy * scale;
			dstRect.w = static_cast<float>(m_widths[frame]);
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
	}

	void Font::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
	{
		m_spriteBatch.Draw(graphics, cameraMatrix);
		m_spriteBatch.Clear();
	}

	bool Font::LoadWidthData(const char* filename)
	{
		unsigned char buffer[512];

		std::ifstream infile;

		infile.open(filename, std::ios::binary);

		if (!infile) return false;

		// Read 512 byte, 2 byte per character
		infile.read(reinterpret_cast<char *>(&buffer), 512);

		if (infile.bad()) return false;
		infile.close();

		for (int i = 0; i < 256; i++)
			m_widths[i] = static_cast<int>(buffer[i * 2]);
		

		return true;
	}

	bool Font::LoadBitmapFont(const char* filename)
	{
		m_fontTexture.LoadFromFile(filename);
		m_spriteBatch.SetTexture(m_fontTexture);
		return true;
	}
};