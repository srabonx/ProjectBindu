#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Bindu.h"

namespace BINDU
{
	class SceneObject
	{
	private:

		std::vector<SceneObject*> m_childs;

		bool			m_isActive{};

	public:

		SceneObject() = default;
		virtual ~SceneObject() = default;


		virtual void	Update(float dt) = 0;

		void			AddChild(SceneObject* sceneObject);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }



	};//Class
};

#endif // SCENEOBJECT_H
