
#include "Include/Bindu.h"

namespace BINDU
{
	void SceneObject::AddChild(SceneObject* sceneObject)
	{
		m_childs.push_back(sceneObject);
	}

};