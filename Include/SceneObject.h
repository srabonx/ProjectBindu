#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Bindu.h"
#include "Drawable.h"


namespace BINDU
{
	class SceneObject : public Drawable
	{
	private:

		std::vector<std::unique_ptr<SceneObject> > m_childs;

		bool			m_isActive{};
		std::string     m_guid{};

	public:

		SceneObject() = default;
		virtual ~SceneObject() = default;

		virtual void	Update(float dt) = 0;
		virtual void	ProcessInput() = 0;

		void			UpdateWithChild(float dt);

		void			DrawWithChild(Graphics* graphics, const Vec2f& cameraOffset);

		void			ProcessAllInput();

		void			UpdateTransforms();

		void			AddChild(std::unique_ptr<SceneObject> sceneObject, const char* guid);

		std::unique_ptr<SceneObject>	RemoveChild(const char* guid);

		void			Clear();

		SceneObject*	GetChild(const char* guid) const;

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

		inline int		getChildCount() const { return static_cast<int>(m_childs.size()); }



	};//Class
};

#endif // SCENEOBJECT_H
