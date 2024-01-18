
#include "Include/Bindu.h"

namespace BINDU
{
	void Scene::Update(float dt)
	{
		std::vector<SceneObject*>::iterator itr;

		for (itr = m_objects.begin(); itr != m_objects.end(); itr++)
		{
			if ((*itr)->isActive())
			{
				(*itr)->Update(dt);
			}
		}
	}

	void Scene::AddObject(SceneObject* sceneObject)
	{
		m_objects.push_back(sceneObject);
	}
};