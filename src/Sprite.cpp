#include "Include/Bindu.h"

namespace BINDU
{
	Sprite::Sprite()
	{
		m_bitmap = nullptr;
		m_imageLoaded = false;
		setPosition(0.0f, 0.0f);
		setVelocity(0.0f, 0.0f);
		m_state = 0;
		m_direction = 0;


		m_width = 1;
		m_height = 1;
		m_scale = 1;
		m_rotation = 0;
		m_doesRotate = false;
		m_rotationTime = 0;
		m_rotationDelta = 0;
		m_translateX = 0;
		m_translateY = 0;
		m_center = D2D1::Point2F(0, 0);
		m_totalFrame = 0;
		m_totalColumn = 0;
		m_currentFrame = 0;
		m_timeCount = 0;
		m_moveTimer = 16;
		m_moveStart = 0;
		m_animFrameWidth = 1;
		m_animFrameHeight = 1;
		m_animated = false;

		m_collideable = true;
		m_collisionType = COLLISION_RECT;
	
		
		srcRect = { 0,0,0,0 };
	}
	Sprite::~Sprite() 
	{
		 
		if (m_animationMap.size() >0) {

			m_animationMap.clear();
		}
		
	}

	

    void Sprite::Update(float dt)
	{
		updateTransform();

			Animate();

		if (m_moveTimer > 0)
		{
			if (GetTickCount() > (DWORD)(m_moveStart + m_moveTimer))
			{
				// reset move timer
				m_moveStart = GetTickCount();
				m_position.x += m_velocity.x * dt;
				m_position.y += m_velocity.y * dt;
			}
		}
		else
		{
			// No movement timer
			m_position.x += m_velocity.x * dt;
			m_position.y += m_velocity.y * dt;
		}

		if (m_doesRotate) {

			if (m_rotationTimer.stopwatch(m_rotationTime))
			{
				m_rotation += m_rotationDelta;

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
				m_scale += m_scaleDelta;

				if (m_scale > m_maxScale || m_scale < m_minScale)
					m_scaleDelta *= -1;
			}

		}


	}

	bool Sprite::LoadFromFile(const wchar_t* filename)
	{

		if (!BitmapLoader::LoadFromFile(filename,m_bitmap.ReleaseAndGetAddressOf()))
		{
			g_engine->sendMessage("Error loading bitmap!", "Error", e_Error::FATAL_ERROR);
			return false;
		}

	    m_bitmapSize = m_bitmap->GetSize();

		m_imageLoaded = true;
		return true;
	}

	bool Sprite::LoadAnimationFile(const char* filename)
	{
		std::ifstream myFile;
		AnimDetails animDetails;
		myFile.open(filename);
		std::string frame_height;
		std::string frame_width;
		std::string state;
		std::string index;
		std::string framecount;
		std::string buffer;

		if (myFile.is_open()) {
			char myChar;
			while (myFile) {
				myChar = myFile.get();
				if (myChar != ' ' && myChar != '\n') {
					if (myChar != '=' && myChar != ':' && myChar != ',' && !isdigit(myChar) && myChar != ';' && myChar != '-')
						buffer += myChar;

					if (buffer == "frame_width")
					{
						if (isdigit(myChar))
							frame_width += myChar;
						if (myChar == ',')
							buffer.clear();
					}
					if (buffer == "frame_height")
					{
						if (isdigit(myChar))
							frame_height += myChar;
						if (myChar == ',')
							buffer.clear();
					}

					if (myChar == '-')
					{
						state = buffer;
						buffer.clear();
					}
					if (buffer == "index")
					{
						if (isdigit(myChar))
							index += myChar;
						if (myChar == ',')
							buffer.clear();
					}

					if (buffer == "frame_count")
					{
						if (isdigit(myChar))
							framecount += myChar;
						if (myChar == ',')
							buffer.clear();
					}

					if (myChar == ';')//|| myChar == EOF)
					{
						m_animFrameWidth = static_cast<float>(std::atoi(frame_width.c_str()));
						m_animFrameHeight = static_cast<float>(std::atoi(frame_height.c_str()));
						animDetails.index = std::atoi(index.c_str());
						animDetails.framecount = std::atoi(framecount.c_str());
						m_animationMap[state] = animDetails;
						state.clear();
						index.clear();
						framecount.clear();
						buffer.clear();
					}
				}
			}
		}
		else
			return false;

		myFile.close();
		return true;
	}

	void Sprite::SetBitmap(ID2D1Bitmap* bitmap)
	{
		m_bitmap = bitmap;

		m_bitmapSize = m_bitmap->GetSize();

		m_imageLoaded = true;
	}

	void Sprite::updateTransform()
	{
		m_center = D2D1::Point2F(m_position.x + (m_width / 2.0f), m_position.y + (m_height / 2.0f));

		m_translateMatrix = D2D1::Matrix3x2F::Translation(m_translateX, m_translateY);
		m_rotationMatrix = D2D1::Matrix3x2F::Rotation(m_rotation, m_center);
		m_scalingMatrix = D2D1::Matrix3x2F::Scale(m_scale, m_scale, m_center);
	}

	void Sprite::Move()
	{

	}

	void Sprite::setAnimation(const char* animation)
	{
		m_currentRow = m_animationMap[animation].index;
		m_totalFrame = m_animationMap[animation].framecount;
	}

	void Sprite::Draw(Graphics* graphics)
	{

		graphics->getRenderTarget()->SetTransform(m_scalingMatrix * m_rotationMatrix * m_translateMatrix);
	
		graphics->getRenderTarget()->DrawBitmap(m_bitmap.Get(),
			D2D1::RectF(m_position.x, m_position.y, m_position.x + m_width, m_position.y + m_height),
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			srcRect);
		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
		
	}

	void Sprite::DrawSub(D2D1_RECT_F rect, Graphics* graphics)
	{
		graphics->getRenderTarget()->SetTransform(m_scalingMatrix * m_rotationMatrix * m_translateMatrix);

		graphics->getRenderTarget()->DrawBitmap(m_bitmap.Get(),
			D2D1::RectF(m_position.x, m_position.y, m_position.x + rect.right, m_position.y + rect.bottom),
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			rect);
		graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void Sprite::Animate()
	{
		float fx, fy;

		if (m_animTimer.stopwatch(m_timeCount) && m_animated)
		{
			m_currentFrame++;
			if (m_currentFrame <= 0) m_currentFrame = m_totalFrame;
			if (m_currentFrame >= m_totalFrame)m_currentFrame = 0;
		}

		if (m_totalFrame > 1 && m_totalColumn > 1)
		{
			fx = (m_currentFrame % m_totalColumn) * m_animFrameWidth;
			fy = (m_currentFrame / m_totalColumn) * m_animFrameHeight;
		}
		else
		{
			fx = std::round(m_currentFrame * m_animFrameWidth);
			fy = std::round(m_currentRow * m_animFrameHeight);
		}
		srcRect = { fx,fy,fx + m_animFrameWidth,fy + m_animFrameHeight };
	}

};// Namespace