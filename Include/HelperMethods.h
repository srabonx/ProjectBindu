#include "Bindu.h"
#pragma once

using namespace BINDU;
#define E_INSTANCE Engine::GetInstance()

// Helper methods of Engine
namespace HM {

	struct ColorValue
	{
		float r; float g; float b; float a = 1.0f;
	};

	inline Vec2i GetWindowSize()
	{
		//return { Engine::GetInstance()->getWindowWidth(),Engine::GetInstance()->getWindowHeight() };
		return { (int)E_INSTANCE->getGraphics()->getRenderTarget()->GetSize().width,(int)E_INSTANCE->getGraphics()->getRenderTarget()->GetSize().height };
	}



	// Primitive shapes
	struct Rect
	{
		int x; int y; int width; int height;
		ColorValue color;
	};
	struct RectF
	{
		float x; float y; float width; float height;
		ColorValue color;
	};



	// Helper methods of Graphics
	

	void Clear(D2D1::ColorF color = D2D1::ColorF::White)
	{
		E_INSTANCE->getGraphics()->Clear(color);
	}

	void DrawLine(Vec2f point1, Vec2f point2, D2D1::ColorF color = D2D1::ColorF::AliceBlue)
	{
		ID2D1SolidColorBrush* colorBrush = E_INSTANCE->getGraphics()->getSolidColorBrush();
		colorBrush->SetColor(color);

		E_INSTANCE->getGraphics()->getRenderTarget()->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(point1.x), static_cast<FLOAT>(point1.y)),
			D2D1::Point2F(static_cast<FLOAT>(point2.x), static_cast<FLOAT>(point2.y)),
			colorBrush
		);

	}

	void DrawFilledRectF(float x, float y, float width, float height, D2D1::ColorF color = D2D1::ColorF::White)
	{
		D2D1_RECT_F rect = D2D1::RectF(
			x, y, x + width, y + height
		);

		ID2D1SolidColorBrush* colorBrush = E_INSTANCE->getGraphics()->getSolidColorBrush();

		colorBrush->SetColor(color);

		E_INSTANCE->getGraphics()->getRenderTarget()->FillRectangle(rect, colorBrush);

	}

	void DrawFilledRectF(const RectF& rect, D2D1::ColorF color = D2D1::ColorF::White)
	{
		DrawFilledRectF(rect.x, rect.y, rect.width, rect.height, color);
	}

	void DrawFilledRectF(const RectF& rect, const ColorValue& color)
	{
		DrawFilledRectF(rect.x, rect.y, rect.width, rect.height, D2D1::ColorF(color.r, color.g, color.b, color.a));
	}

	void DrawRectF(float x, float y, float width, float height, D2D1::ColorF color = D2D1::ColorF::White)
	{
		D2D1_RECT_F rect = D2D1::RectF(
			x, y, x + width, y + height
		);

		ID2D1SolidColorBrush* colorBrush = E_INSTANCE->getGraphics()->getSolidColorBrush();

		colorBrush->SetColor(color);

		E_INSTANCE->getGraphics()->getRenderTarget()->DrawRectangle(rect, colorBrush);

	}

	void DrawRectF(const RectF& rect, D2D1::ColorF color = D2D1::ColorF::White)
	{
		DrawRectF(rect.x, rect.y, rect.width, rect.height, color);
	}

	void DrawRectF(const RectF& rect, const ColorValue& color)
	{
		DrawRectF(rect.x, rect.y, rect.width, rect.height, D2D1::ColorF(color.r, color.g, color.b, color.a));
	}

};


