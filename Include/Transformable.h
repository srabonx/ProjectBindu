#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H
#include <d2d1.h>

#include "BND_Helper.h"
#include "Vector.h"

namespace BINDU
{
	class Transformable
	{
	protected:

		Vec2f					m_position{};
		Bnd_Size_F				m_size{};

		Vec2f					m_translation{};
		Vec2f					m_scale{ 1,1 };
		float					m_rotation{};

		D2D1_MATRIX_3X2_F		m_transform{};

		D2D1_POINT_2F			m_origin{};



	public:

		Transformable() = default;
		virtual ~Transformable() = default;

		virtual void			UpdateTransform();

		inline void				setTransform(const D2D1_MATRIX_3X2_F& transform) { m_transform = transform; }
		inline D2D1_MATRIX_3X2_F getTransform() const { return m_transform; }

		Vec2f					getCenter() const;

		inline void				setOrigin(const float x, const float y) { m_origin.x = x; m_origin.y = y; }
		inline void				setOrigin(const Vec2f& origin) { m_origin = { origin.x,origin.y }; }
		inline Vec2f			getOrigin() const { return { m_origin.x,m_origin.y }; }

		inline void				setTranslation(const float dx, const float dy) { m_translation.x = dx; m_translation.y = dy; }
		inline void				setTranslation(const Vec2f& translation) { m_translation = translation; }
		inline Vec2f			getTranslation() const { return {m_transform.dx, m_transform.dy}; }

		inline void				setRotation(const float degree) { m_rotation = degree; }
		inline float			getRotation() const { return m_rotation; }

		inline void				setScale(const float x, const float y) { m_scale = { x,y }; }
		inline void				setScale(const Vec2f& scale) { m_scale = scale; }
		inline void				scaleX(const float x) { m_scale.x = x; }
		inline void				scaleY(const float y) { m_scale.y = y; }
		inline Vec2f			getScale() const { return m_scale; }

		inline void				setPosition(const Vec2f& position) { m_position = position; }
		inline void				setPosition(const float x, const float y) { m_position = { x,y }; }
		inline void				setX(const float x) { m_position.x = x; }
		inline void				setY(const float y) { m_position.y = y; }
		inline Vec2f			getPosition() const { return m_position; };
		inline void				getPosition(float& x, float& y) const { x = m_position.x; y = m_position.y; }
		inline float			getX() const { return m_position.x; }
		inline float			getY() const { return m_position.y; }

		inline void				setSize(const Bnd_Size_F& size) { m_size = size; }
		inline void				setSize(const float width, const float height) { m_size = { width,height }; }
		inline void				setWidth(const float width) { m_size.width = width; }
		inline void				setHeight(const float height) { m_size.height = height; }
		inline Bnd_Size_F		getSize() const { return m_size; }
		inline void				getSize(float& width, float& height) const { width = m_size.width; height = m_size.height; }
		inline float			getWidth() const { return m_size.width; }
		inline float			getHeight() const { return m_size.height; }

	};
};

#endif // TRANSFORMABLE_H
