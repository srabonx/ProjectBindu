#include "Include/Bindu.h"

namespace BINDU
{
	SpriteBatch::SpriteBatch()
	{
		m_spriteBatch = nullptr;
		m_deviceContxt = nullptr;
	}

	SpriteBatch::~SpriteBatch()
	{

	}

	void SpriteBatch::Init()
	{
		HRESULT hr = S_OK;

 		DX::ThrowIfFailed(
 
 			g_engine->getGraphics()->getD2dDeviceContext()->QueryInterface(m_deviceContxt.ReleaseAndGetAddressOf())
 		
 		);

 		DX::ThrowIfFailed(
 
 			m_deviceContxt->CreateSpriteBatch(m_spriteBatch.ReleaseAndGetAddressOf())
 
 		);

	}

	void SpriteBatch::LoadSpriteSheet(const wchar_t* filename)
	{
		LoadFromFile(filename);
	}

	void SpriteBatch::setSpriteCount(int value)
	{
		m_spriteCount = value;

	}

	void SpriteBatch::SetSprite( const D2D1_RECT_F& dstRect, D2D1_RECT_U* srcRect, const BND_COLOR& color, float scale , float rotation, Vec2f velocity)
	{
		m_velocities.push_back(velocity);

		D2D1_COLOR_F lcolor{};
		lcolor.r = ( color.r / 255.f );
		lcolor.g = ( color.g / 255.f );
		lcolor.b = ( color.b / 255.f );
		lcolor.a = ( color.a / 255.f );

		D2D1_POINT_2F center = D2D1::Point2F(dstRect.left + ((dstRect.right - dstRect.left) / 2),
											 dstRect.top + ((dstRect.bottom - dstRect.top) / 2));



		D2D1_MATRIX_3X2_F rotationMatrix = D2D1::Matrix3x2F::Rotation(rotation,center);
		D2D1_MATRIX_3X2_F scaleMatrix = D2D1::Matrix3x2F::Scale(scale, scale,center);

		
		D2D1_MATRIX_3X2_F transformMatrix = (scaleMatrix * rotationMatrix);

		m_spriteBatch->AddSprites(1, &dstRect, NULL, &lcolor, &transformMatrix);
	
	}

	void SpriteBatch::StartSpriteBatch()
	{

	}

	void SpriteBatch::Update(float dt)
	{
		UINT32 count = m_spriteBatch->GetSpriteCount();


		for (UINT32 i = 0; i < count; i++)
		{
			D2D1_RECT_F dstRect{};
			D2D1_RECT_U srcRect{};
			D2D1_COLOR_F lcolor{};
			D2D1_MATRIX_3X2_F transfromMatrix{};

			m_spriteBatch->GetSprites(i, 1, &dstRect, &srcRect, &lcolor, &transfromMatrix);

			D2D1_POINT_2F center = D2D1::Point2F(dstRect.left + ((dstRect.right - dstRect.left) / 2),
					dstRect.top + ((dstRect.bottom - dstRect.top) / 2));

			transfromMatrix = transfromMatrix * D2D1::Matrix3x2F::Translation( m_velocities[i].x * dt, m_velocities[i].y * dt);

			m_spriteBatch->SetSprites(i, 1, &dstRect, &srcRect, &lcolor, &transfromMatrix);

		}

	}

	void SpriteBatch::Draw(Graphics* graphics)
	{
		m_deviceContxt->DrawSpriteBatch(m_spriteBatch.Get(), m_bitmap.Get());
	}
};// namespace