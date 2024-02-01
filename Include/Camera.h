#ifndef CAMERA_H
#define CAMERA_H

#include <d2d1.h>

#include "SceneObject.h"
#include "Vector.h"

namespace BINDU
{
	class Camera
	{
	private:

		float	   m_zoom{ 1 };
		float	   m_rotation{ 0 };
		Vec2f      m_position{};
		D2D1_MATRIX_3X2_F m_transformMatrix{};

		Vec2f	   m_offset{};

		D2D1_POINT_2F* m_target = nullptr;

	public:
		Camera() = default;
		~Camera() = default;

		void	SetTarget(const Vec2f& target);

		void	Update(float dt,const Vec2f& target);

		inline D2D1_MATRIX_3X2_F getCameraMatrix() const { return m_transformMatrix; }

		inline void		setZoom(float value)
		{
			m_zoom = value;
			if (value < 0.1f) m_zoom = 0.1f;
		}

		inline float	getZoom() const { return m_zoom; }

		inline void		setRotation(float value) { m_rotation = value; }
		inline float	getRotation() const { return m_rotation; }



	};

	
};

#endif // CAMERA_H
