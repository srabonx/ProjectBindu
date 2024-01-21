
#include "Include/Bindu.h"

namespace BINDU
{
	void Camera::SetTarget(SceneObject& sceneObject)
	{
		m_target = sceneObject.getD2Origin();
	}

	void Camera::Update(float dt, const SceneObject& sceneObject)
	{
		m_offset.x = (m_target->x - (g_engine->getWindowWidth()  * 0.5f));
		m_offset.y = (m_target->y - (g_engine->getWindowHeight() * 0.5f));

		if (m_offset.x < 0)
			m_offset.x = 0;
		if (m_offset.y < 0)
			m_offset.y = 0;

		m_position.x = (m_offset.x + (m_position.x - m_offset.x) * 0.93f);
		m_position.y = (m_offset.y + (m_position.y - m_offset.y) * 0.95f);


		m_transformMatrix = D2D1::Matrix3x2F::Translation(-m_position.x , -m_position.y ) *
			D2D1::Matrix3x2F::Rotation(m_rotation, *m_target)*
			D2D1::Matrix3x2F::Scale(m_zoom, m_zoom, *m_target);
	}
};
