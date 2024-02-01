#include "Include/Sprite.h"


namespace BINDU
{
	Sprite::Sprite()
	{
		m_rotation = 0;	
	}

	Sprite::Sprite(const Texture& texture, const Bnd_Rect_F& rect)
	{
		SetTexture(texture);
		setRect(rect);
	}


	void Sprite::SetTexture(const Texture& texture)
	{
		SetBitmap(texture.getBitmap());
		m_srcRect = { 0.f,0.f,texture.getBitmap()->GetSize().width,texture.getBitmap()->GetSize().height };
	}

    void Sprite::Update(float dt)
	{


	}


	void Sprite::setRect(const Bnd_Rect_F& rect)
	{
		m_srcRect = { rect.x,rect.y,rect.x + rect.w,rect.y + rect.h };
	}

	Bnd_Rect_F Sprite::getRect() const
	{
		return { m_srcRect.left,m_srcRect.top,m_srcRect.right - m_srcRect.left,m_srcRect.bottom - m_srcRect.top };
	}

	void Sprite::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
	{
		UpdateTransform();

		graphics->getRenderTarget()->SetTransform(m_transform * cameraMatrix);
		
		graphics->getRenderTarget()->DrawBitmap(m_bitmap.Get(),
			{ m_position.x,m_position.y,(m_position.x + m_size.width),(m_position.y + m_size.height) },
			m_opacity,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			m_srcRect);

		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		
	}

	void Sprite::Draw(Bnd_Rect_F srcRect, Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
	{
		UpdateTransform();

		graphics->getRenderTarget()->SetTransform(m_transform * cameraMatrix);

		graphics->getRenderTarget()->DrawBitmap(m_bitmap.Get(),
			{ m_position.x ,m_position.y ,(m_position.x + m_size.width) ,(m_position.y + m_size.height) },
			m_opacity,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(srcRect.x, srcRect.y, srcRect.x + srcRect.w, srcRect.y + srcRect.h));

		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
	}


};// Namespace