#ifndef FONT_H
#define FONT_H
#include <d2d1.h>

#include "Graphics.h"
#include "SpriteBatch.h"

namespace BINDU
{
	class Font final :private Drawable
	{
	private:
		Texture m_fontTexture;
		int		m_widths[256]{ };

		SpriteBatch m_spriteBatch;

		int		m_totalColumn{};
		Bnd_Size_F m_charSize{};

		Bnd_Size_F m_srcCharSize{};

	public:
		Font();
		~Font() = default;

		void Init();

		void PrintText(int x, int y, const std::string& text, const BND_COLOR& color = { 255,255,255,255 }, float scale = 1);

		void Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;

		inline void setCharWidth(const float width)
		{
			m_charSize.width = width;
		}

		inline void setCharHeight(const float height)
		{
			m_charSize.height = height;
		}

		inline void setCharSize(const float width, const float height)
		{
			m_charSize = { width , height };
		}

		bool LoadWidthData(const char* filename);
		bool LoadBitmapFont(const char* filename);

	};

	
};
#endif // FONT_H
