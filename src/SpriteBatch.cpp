#include "Include/Bindu.h"

namespace BINDU
{
	SpriteBatch::SpriteBatch()
	{
		m_coEff = 1.0f / 255.f;
	}

	SpriteBatch::~SpriteBatch()
	{

	}

	void SpriteBatch::Init()
	{
		HRESULT hr = S_OK;

 		DX::ThrowIfFailed(
 
 			g_engine->getGraphics()->getD2dDeviceContext()->QueryInterface(m_deviceContext.ReleaseAndGetAddressOf())
 		
 		);

 		DX::ThrowIfFailed(
 
 			m_deviceContext->CreateSpriteBatch(m_spriteBatch.ReleaseAndGetAddressOf())
 
 		);

	}

	void SpriteBatch::LoadSpriteSheet(const wchar_t* filename)
	{
		LoadFromFile(filename);
	}

	void SpriteBatch::AddSprite( const Bnd_Rect_F& dstRect,const Bnd_Rect* srcRect, const BND_COLOR& color, float scale , float rotation)
	{

		D2D1_COLOR_F lColor{};
		lColor.r = ( color.r * m_coEff);
		lColor.g = ( color.g * m_coEff);
		lColor.b = ( color.b * m_coEff);
		lColor.a = ( color.a * m_coEff);

		D2D1_RECT_F lDstRect{};

		lDstRect.left = dstRect.x;
		lDstRect.top = dstRect.y;
		lDstRect.right = dstRect.x + dstRect.w;
		lDstRect.bottom = dstRect.y + dstRect.h;

		D2D1_RECT_U lSrcRect{};
		if (srcRect)
		{
			lSrcRect.left = srcRect->x;
			lSrcRect.top = srcRect->y;
			lSrcRect.right = srcRect->x + srcRect->w;
			lSrcRect.bottom = srcRect->y + srcRect->h;
		}
		else
		{
			lSrcRect.left = 0;
			lSrcRect.top = 0;
			lSrcRect.right = m_bitmapSize.width;
			lSrcRect.bottom = m_bitmapSize.height;
		}

		m_origin = { lDstRect.left + ((lDstRect.right - lDstRect.left) / 2.f), lDstRect.top + ((lDstRect.right - lDstRect.left) / 2.f) };


		m_rotationMatrix = D2D1::Matrix3x2F::Rotation(rotation,m_origin);
		m_scalingMatrix = D2D1::Matrix3x2F::Scale(scale, scale, m_origin);

		m_transforms = m_scalingMatrix * m_rotationMatrix;

		DX::ThrowIfFailed(
			m_spriteBatch->AddSprites(1, &lDstRect, &lSrcRect, &lColor, &m_transforms)
		);
	
	}

	void SpriteBatch::UpdateSprite(int index,int count, const Bnd_Rect_F* dstRect, const Bnd_Rect* srcRect, const BND_COLOR& newColor, float newScale, float newRotation)
	{

		D2D1_COLOR_F lColor{};
		lColor.r = (newColor.r * m_coEff);
		lColor.g = (newColor.g * m_coEff);
		lColor.b = (newColor.b * m_coEff);
		lColor.a = (newColor.a * m_coEff);

//		D2D1_RECT_F ldstRect{};

//		m_spriteBatch->GetSprites(index, 1, &ldstRect,nullptr,nullptr,nullptr);

//		D2D1_RECT_F dstRect{};

/*		dstRect.left = newPosition.x;
		dstRect.top = newPosition.y;
		dstRect.right = newPosition.x + (ldstRect.right - ldstRect.left);
		dstRect.bottom = newPosition.y + (ldstRect.bottom - ldstRect.top); */

//		ldstRect = dstRect;


		D2D1_RECT_F lDstRect{};

		lDstRect.left = dstRect->x;
		lDstRect.top = dstRect->y;
		lDstRect.right = dstRect->x + dstRect->w;
		lDstRect.bottom = dstRect->y + dstRect->h;

		D2D1_RECT_U* lSrcRect{};

		if (srcRect)
		{
			lSrcRect->left = srcRect->x;
			lSrcRect->top = srcRect->y;
			lSrcRect->right = srcRect->x + srcRect->w;
			lSrcRect->bottom = srcRect->y + srcRect->h;
		}
		else
			lSrcRect = nullptr;

		m_origin = { lDstRect.left + ((lDstRect.right - lDstRect.left) / 2.f), lDstRect.top + ((lDstRect.right - lDstRect.left) / 2.f) };
		m_rotationMatrix = D2D1::Matrix3x2F::Rotation(newRotation, m_origin);
		m_scalingMatrix = D2D1::Matrix3x2F::Scale(newScale, newScale, m_origin);

		m_transforms = m_scalingMatrix * m_rotationMatrix;

		DX::ThrowIfFailed(
			m_spriteBatch->SetSprites(index, 1, &lDstRect, lSrcRect, &lColor, &m_transforms)
		);

	}

	void SpriteBatch::BatchAdd(int count, const Bnd_Rect_F& dstRect)
	{

		//m_dstRect = dstRect();

		//DX::ThrowIfFailed(
		//	m_spriteBatch->AddSprites(count, &m_dstRect, nullptr, nullptr, nullptr)
		//);

	}

	void SpriteBatch::Clear() const
	{
		m_spriteBatch->Clear();
	}

	void SpriteBatch::Draw(Graphics* graphics, int index, int count) const
	{
		m_deviceContext->DrawSpriteBatch(m_spriteBatch.Get(), index, count, m_bitmap.Get());
	}

	void SpriteBatch::Update(float dt)
	{

	}

	void SpriteBatch::Draw(Graphics* graphics)
	{
		m_deviceContext->DrawSpriteBatch(m_spriteBatch.Get(), m_bitmap.Get());
	}
};// namespace