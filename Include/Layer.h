#ifndef LAYER_H
#define LAYER_H

#include "Bindu.h"

namespace BINDU
{
	class Layer
	{
	private:

		bool	m_isActive{};

		std::string m_guid{};

		std::vector<std::unique_ptr<SceneObject>> m_objects;

	public:
		
		Layer() = default;
		~Layer() = default;

		void			Update(float dt) const;
		void			Draw(Graphics* graphics) const;
		void			ProcessInput() const;

		void			AddObject(std::unique_ptr<SceneObject> sceneObject, const char* guid);

		std::unique_ptr<SceneObject> RemoveObject(const char* guid);

		SceneObject*	getObject(const char* guid);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline int		getObjectCount() const { return static_cast<int>(m_objects.size()); }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

	};

};


#endif // LAYER_H
