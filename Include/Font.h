#ifndef FONT_H
#define FONT_H

#include "Bindu.h"

#pragma once

namespace BINDU
{
	class Font final :public Drawable
	{
	private:
		int		m_widths[256]{ };

		SpriteBatch m_spriteBatch;

		int		m_totalColumn{};
		Bnd_Size_F m_charSize{};

	public:
		Font();
		~Font() = default;

		void Init();

		void PrintText(int x, int y, const std::string& text, const BND_COLOR& color, float scale);

		virtual void Draw(Graphics* graphics, const Vec2f& cameraOffset) override
		{
			
		}

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
		bool LoadBitmapFont(const wchar_t* filename);

	};

	
};
#endif // FONT_H
