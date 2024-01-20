
#include "Include/Bindu.h"

namespace BINDU
{
	void Camera::SetTarget(SceneObject& sceneObject)
	{
		target = sceneObject.getD2Origin();
	}

	void Camera::Update(float dt)
	{
		if(target != nullptr)
		{
			m_viewPort.x = target->x - (1280.f / 2.f);
			m_viewPort.y = target->y - (800.f / 2.f);

			

			m_offset = { m_viewPort.x,m_viewPort.y };
		}
	}
};
