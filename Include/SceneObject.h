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

	enum class ColliderType
	{
		RECT_COLLIDER,
		CIRCLE_COLLIDER,
	};

	class SceneObject : public Drawable, public Transformable
	{
	private:

		std::vector<std::unique_ptr<SceneObject> > m_childs;

		bool			m_isActive{};

		bool			m_doDestroy{ false };

		std::string     m_guid{};

	protected:

		Bnd_Rect_F      m_rectCollider{};

		Bnd_Circle		m_circleCollider{};

		ColliderType	m_colliderType = ColliderType::RECT_COLLIDER;

		Bnd_Rect_F		m_colliderOffsetRect{};

		float			m_colliderOffsetRadius{};

		bool			m_collideAble{};

	public:

		SceneObject() = default;
		virtual ~SceneObject() = default;

		virtual void	Update(float dt) = 0;
		virtual void	ProcessInput() = 0;
		virtual void    CheckCollision(SceneObject& sceneObject){}
		virtual void	onLoadResource();
		virtual void	onReleaseResource();


		void			UpdateWithChild(float dt);

		void			DrawWithChild(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix);

		void			ProcessAllInput();

		void			AddChild(std::unique_ptr<SceneObject> sceneObject, const char* guid);

		std::unique_ptr<SceneObject>	RemoveChild(const char* guid);

		void			Clear();

		SceneObject*	GetChild(const char* guid) const;

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline void     queueForDestroy(const bool destroy) { m_doDestroy = destroy; }

		inline bool		isQueuedForDestroy() const { return m_doDestroy; }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

		inline int		getChildCount() const { return static_cast<int>(m_childs.size()); }

		inline void		setCollider(const Bnd_Rect_F& collider) { m_colliderOffsetRect = collider; }

		inline Bnd_Rect_F getRectCollider() const { return m_rectCollider; }

		inline void		setCircleColliderRadius(const float radius) { m_colliderOffsetRadius = radius; }

		inline Bnd_Circle	getCircleCollider() const { return m_circleCollider; }

		inline void		setCollideAble(const bool value) { m_collideAble = value; }

		inline bool		isCollideAble() const { return m_collideAble; }

		inline void		setColliderType(const ColliderType& colliderType) { m_colliderType = colliderType; }
		inline ColliderType getColliderType() const { return m_colliderType; }


	};//Class
};

#endif // SCENEOBJECT_H
