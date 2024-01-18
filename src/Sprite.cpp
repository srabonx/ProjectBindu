#include "Include/Bindu.h"

namespace BINDU
{
	Sprite::Sprite()
	{

		m_rotation = 0;
		m_doesRotate = false;
		m_rotationTime = 0;
		m_rotationDelta = 0;

		m_collideable = true;
		m_collisionType = COLLISION_RECT;
	
	}
	Sprite::~Sprite() 
	{
		
	}

	

    void Sprite::Update(float dt)
	{

		if (m_doesRotate) {

			if (m_rotationTimer.stopwatch(m_rotationTime))
			{
				m_rotation += m_rotationDelta * dt;

				if (m_rotation > 360)
				{
					m_rotation -= 360;
				}
			}
		}

		if (m_doesScale)
		{
			if (m_scaleTimer.stopwatch(m_scaleTime))
			{
				m_scale += m_scaleDelta * dt;

				if (m_scale.x > m_maxScale || m_scale.x < m_minScale)
					m_scaleDelta *= -1;
			}

		}

	}



	void Sprite::updateTransform()
	{
		m_origin = { m_position.x + (m_size.width / 2.f), m_position.y + (m_size.height / 2.f) };

		//m_translateMatrix = D2D1::Matrix3x2F::Translation(0, 0);				// TODO: Add functionality?
		m_rotationMatrix = D2D1::Matrix3x2F::Rotation(m_rotation, m_origin);
		m_scalingMatrix = D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y, m_origin);
	}

	void Sprite::Draw(Graphics* graphics)
	{
		updateTransform();

		graphics->getRenderTarget()->SetTransform(m_scalingMatrix * m_rotationMatrix );

		graphics->getRenderTarget()->DrawBitmap(m_bitmap.Get(),
			{ m_position.x,m_position.y,m_position.x + m_size.width,m_position.y + m_size.height },
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			{ m_position.x,m_position.y,m_position.x + m_bitmapSize.width,m_position.y + m_bitmapSize.height });

		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		
	}

	void Sprite::Draw(Bnd_Rect_F srcRect, Graphics* graphics)
	{
		updateTransform();

		graphics->getRenderTarget()->SetTransform(m_scalingMatrix * m_rotationMatrix );

		graphics->getRenderTarget()->DrawBitmap(m_bitmap.Get(),
			{ m_position.x,m_position.y,m_position.x + m_size.width,m_position.y + m_size.height },
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			D2D1::RectF(srcRect.x, srcRect.y, srcRect.x + srcRect.w, srcRect.y + srcRect.h));

		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
	}


};// Namespace