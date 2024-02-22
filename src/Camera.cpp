
#include "Include/Camera.h"

#include "Include/Bindu.h"

namespace BINDU
{

	void Camera::Update(float dt, const Vec2f& target)
	{
		m_offset.x = (target.x - (g_engine->getWindowWidth() * 0.5f)) / m_zoom.x;
		m_offset.y = (target.y - (g_engine->getWindowHeight() * 0.5f)) / m_zoom.y;

		if (m_offset.x < m_topLeft.x)
			m_offset.x = m_topLeft.x;
		if (m_offset.y < m_topLeft.y)
			m_offset.y = m_topLeft.y;
		if (m_offset.x + m_viewSize.width > m_bottomRight.x)
			m_offset.x = m_bottomRight.x - m_viewSize.width;
		if (m_offset.y + m_viewSize.height > m_bottomRight.y)
			m_offset.y = m_bottomRight.y - m_viewSize.height;

	/*	m_position.x = (m_offset.x + (m_position.x - m_offset.x) * 0.93f);
		m_position.y = (m_offset.y + (m_position.y - m_offset.y) * 0.95f);*/

		m_position.x = (m_position.x + (m_offset.x - m_position.x) * 0.07f);
		m_position.y = (m_position.y + (m_offset.y - m_position.y) * 0.07f);

		const D2D1_POINT_2F targetPoint = { target.x,target.y };

		m_transformMatrix =	D2D1::Matrix3x2F::Scale(m_zoom.x, m_zoom.y, targetPoint) *
							D2D1::Matrix3x2F::Rotation(m_rotation, targetPoint) *
							D2D1::Matrix3x2F::Translation(-m_position.x, -m_position.y);
	}
};
