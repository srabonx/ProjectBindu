
#include  "Include/Bindu.h"

namespace BINDU
{
	void SceneManager::Update(float dt)
	{
		std::vector<Scene>::iterator itr;

		for (itr = m_scenes.begin(); itr != m_scenes.end(); itr++)
		{
			if(itr->isActive())
			{
				itr->Update(dt);
			}
		}

	}

	void SceneManager::AddScene(const Scene& scene)
	{
		m_scenes.push_back(scene);
	}

};
