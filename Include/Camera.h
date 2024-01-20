#ifndef CAMERA_H
#define CAMERA_H

#include "Bindu.h"
#include "BND_Helper.h"

namespace BINDU
{
	class Camera
	{
	private:

		Bnd_Rect_F m_viewPort{};
		Vec2f	   m_target{};
		D2D1_POINT_2F* target = nullptr;

		D2D1_MATRIX_3X2_F m_translateMatrix;

		Vec2f	   m_offset{};

	public:
		Camera() = default;
		~Camera() = default;

		inline void	SetSize(const Bnd_Size_F& size) { m_viewPort.w = size.width; m_viewPort.h = size.height; }

		void	SetTarget(SceneObject& sceneObject);

		void	Update(float dt);

		inline D2D1_MATRIX_3X2_F getTranslationMatrix() const { return m_translateMatrix; }

		inline Vec2f	getOffset() const { return m_offset; }
	};

	
};

#endif // CAMERA_H
