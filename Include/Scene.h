#ifndef SCENE_H
#define SCENE_H

#include "Bindu.h"

namespace BINDU
{
	class Scene
	{
	private:


		bool	m_isActive{};
		int		m_totalObjects{};

		std::vector<SceneObject*> m_objects;

	public:

		Scene() = default;
		~Scene() = default;

		void		Update(float dt);
		void		AddObject(SceneObject* sceneObject);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

	};// Class

};

#endif // SCENE_H
