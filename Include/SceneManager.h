#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Bindu.h"

namespace BINDU
{
	class SceneManager
	{
	private:

		std::vector<Scene>		m_scenes;

	public:

		SceneManager() = default;
		~SceneManager() = default;

		void		 Update(float dt);

		void		 AddScene(const Scene& scene);




	};//Class;

};

#endif // SCENEMANAGER_H
