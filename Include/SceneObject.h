#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>
#include <string>
#include <vector>

#include "Drawable.h"
#include "Graphics.h"
#include "Transformable.h"


namespace BINDU
{
	class SceneObject : public Drawable, public Transformable
	{
	private:

		std::vector<std::unique_ptr<SceneObject> > m_childs;

		bool			m_isActive{};

		std::string     m_guid{};

	protected:

		Bnd_Rect_F      m_collider{};

		Bnd_Rect_F		m_colliderOffsetRect{};

		bool			m_colliderAble{};

	public:

		SceneObject() = default;
		//SceneObject(const SceneObject&) = delete;
		//SceneObject& operator = (const SceneObject&) = delete;
		virtual ~SceneObject() = default;

		virtual void	Update(float dt) = 0;
		virtual void	ProcessInput() = 0;

		void			UpdateWithChild(float dt);

		void			DrawWithChild(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix);

		void			ProcessAllInput();

		void			AddChild(std::unique_ptr<SceneObject> sceneObject, const char* guid);

		std::unique_ptr<SceneObject>	RemoveChild(const char* guid);

		void			Clear();

		SceneObject*	GetChild(const char* guid) const;

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

		inline int		getChildCount() const { return static_cast<int>(m_childs.size()); }

		inline void		setCollider(const Bnd_Rect_F& collider) { m_collider = collider; }

		inline Bnd_Rect_F getCollider() const { return m_collider; }

		inline void		setColliderOffset(float x,float y,float w,float h)
		{
			m_colliderOffsetRect = { x,y,w,h };
		}

		inline void		setCollideAble(const bool value) { m_colliderAble = value; }

		inline bool		isCollideAble() const { return m_colliderAble; }


	};//Class
};

#endif // SCENEOBJECT_H
