#ifndef CAMERA_H
#define CAMERA_H

#include <d2d1.h>

#include "Bindu.h"
#include "SceneObject.h"
#include "Vector.h"

namespace BINDU
{
	class Camera
	{
	private:

		Vec2f	   m_zoom{ 1.f,1.f };
		float	   m_rotation{ 0 };
		Vec2f      m_position{};
		D2D1_MATRIX_3X2_F m_transformMatrix{};

		Vec2f	   m_offset{};

		Vec2f		m_topLeft{ 0.f,0.f };

		Vec2f		m_bottomRight{ static_cast<float>(g_engine->getWindowWidth()), static_cast<float>(g_engine->getWindowHeight()) };

		Bnd_Size_F	m_viewSize{1,1};

		D2D1_POINT_2F m_origin;

	public:
		Camera() = default;
		~Camera() = default;

		void	Update(float dt,const Vec2f& target);

		inline D2D1_MATRIX_3X2_F getCameraMatrix() const { return m_transformMatrix; }

		inline void		setZoom(const Vec2f& value)
		{
			m_zoom = value;
			if (value.x < 0.1f || value.y < 0.1f) m_zoom = { 0.1f,0.1f };
		}

		inline Vec2f	getZoom() const { return m_zoom; }

		inline void		setRotation(float value) { m_rotation = value; }
		inline float	getRotation() const { return m_rotation; }

		inline void		setTopLeft(const float x, const float y) { m_topLeft = { x,y }; }
		inline void		setTopLeft(const Vec2f& topLeft) { m_topLeft = topLeft; }

		inline Vec2f	getTopLeft() const { return m_topLeft; }

		inline void		setBottomRight(const float x, const float y) { m_bottomRight = { x,y }; }
		inline void		setBottomRight(const Vec2f& bottomRight) { m_bottomRight = bottomRight; }

		inline Vec2f	getBottomRight() const { return m_bottomRight; }

		inline void		setViewSize(const float width, const float height) { m_viewSize = { width,height }; }
		inline void		setViewSize(const Bnd_Size_F& viewSize) { m_viewSize = viewSize; }

		inline Bnd_Size_F	getViewSize() const { return m_viewSize; }

		inline void		setOrigin(const Vec2f& point) { m_origin = { point.x,point.y }; }
		inline Vec2f	getOrigin() const { return { m_origin.x,m_origin.y }; }

	};

	
};

#endif // CAMERA_H
