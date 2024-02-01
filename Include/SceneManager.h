#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

namespace BINDU
{
	class SceneManager
	{
	private:

		std::vector< std::unique_ptr<Scene> >		m_scenes;

	public:

		SceneManager() = default;
		~SceneManager() = default;

		void		 Update(float dt) const;
		void		 Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const;
		void		 ProcessInput() const;

		void		 AddScene(std::unique_ptr<Scene> scene, const char* guid);

		std::unique_ptr<Scene> RemoveScene(const char* guid);

		Scene*		 getScene(const char* guid);

		inline int   getSceneCount() const { return static_cast<int>(m_scenes.size()); }


	};//Class;

};

#endif // SCENEMANAGER_H
