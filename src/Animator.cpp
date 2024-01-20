
#include "Include/Bindu.h"

namespace BINDU
{
	bool Animator::LoadAnimationFile(const char* filename)
	{
		std::ifstream myFile;
		AnimDetails animDetails {};

		myFile.open(filename);


		if (myFile.is_open()) {

			std::string frameCount;
			std::string index;
			std::string frameWidth;
			std::string frameHeight;

			std::string buffer;
			std::string state;

			char myChar;
			while (myFile) {
				myChar = myFile.get();
				if (myChar != ' ' && myChar != '\n') {
					if (myChar != '=' && myChar != ':' && myChar != ',' && !isdigit(myChar) && myChar != ';' && myChar != '-')
						buffer += myChar;

					if (buffer == "frame_width")
					{
						if (isdigit(myChar))
							frameWidth += myChar;
						if (myChar == ',')
							buffer.clear();
					}
					if (buffer == "frame_height")
					{
						if (isdigit(myChar))
							frameHeight += myChar;
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
							frameCount += myChar;
						if (myChar == ',')
							buffer.clear();
					}

					if (myChar == ';')//|| myChar == EOF)
					{
						m_frameSize.width = static_cast<float>(std::stoi(frameWidth));
						m_frameSize.height = static_cast<float>(std::stoi(frameHeight));
						animDetails.m_index = std::stoi(index);
						animDetails.m_frameCount = std::stoi(frameCount);
						m_animationMap[state] = animDetails;
						state.clear();
						index.clear();
						frameCount.clear();
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

	void Animator::Animate(Bnd_Rect_F& srcRect)
	{
		float fx, fy;

		if (m_frameTimer.stopwatch(m_frameTimeMs))
		{
			m_currentFrame++;
			if (m_currentFrame <= 0) m_currentFrame = m_totalFrame;
			if (m_currentFrame >= m_totalFrame)m_currentFrame = 0;
		}

		if (m_totalFrame > 1 && m_totalColumn > 1)
		{
			fx = static_cast<float>(m_currentFrame % m_totalColumn) * m_frameSize.width;
			fy = (static_cast<float>(m_currentFrame) / static_cast<float>(m_totalColumn)) * m_frameSize.height;
		}
		else
		{
			fx = std::round((static_cast<float>(m_currentFrame) * m_frameSize.width));
			fy = std::round((static_cast<float>(m_currentRow) * m_frameSize.height));
		}

		srcRect.x = fx;
		srcRect.y = fy;
		srcRect.w = m_frameSize.width;
		srcRect.h = m_frameSize.height;

	}

	void Animator::setAnimation(const char* animation)
	{
		m_currentRow = m_animationMap[animation].m_index;
		m_totalFrame = m_animationMap[animation].m_frameCount;
	}
};