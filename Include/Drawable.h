#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Bindu.h"
#include "BND_Helper.h"

namespace BINDU
{

	enum class origin
	{
		top_left,
		center,
		bottom_left,
		bottom_right

	};

	class Drawable
	{
	protected:

		ComPtr<ID2D1Bitmap>				m_bitmap;

		Vec2f							m_position{};
		Bnd_Size_F						m_size{ 1,1 };

		Bnd_Size						m_bitmapSize{};

		D2D1_POINT_2F					m_origin{};
		origin							m_e_origin{};

		Vec2f							m_scale{ 1,1 };
		float							m_rotation{};

		/*
		 *	TODO: Add Translation property ?
		 */


		D2D1::Matrix3x2F				m_translateMatrix;
		D2D1::Matrix3x2F				m_rotationMatrix;
		D2D1::Matrix3x2F				m_scalingMatrix;



	protected:

		bool		  LoadFromFile(const wchar_t* filename);

	public:

		              Drawable() = default;
		virtual		 ~Drawable() = default;

		virtual void  Draw(Graphics* graphics) = 0;

		void		  SetBitmap(ID2D1Bitmap* bitmap);

		// Accesor/ Mutators

		inline Bnd_Size getBitmapSize() const { return m_bitmapSize; }



		inline void		setPosition(const Vec2f& position) { m_position = position; }
		inline void		setPosition(const float x, const float y) { m_position = { x,y }; }
		inline void		setX(const float x) { m_position.x = x; }
		inline void		setY(const float y) { m_position.y = y; }
		inline Vec2f	getPosition() const { return m_position; };
		inline void		getPosition(float& x, float& y) const { x = m_position.x; y = m_position.y; }
		inline float	getX() const { return m_position.x; }
		inline float	getY() const { return m_position.y; }

		inline void		setSize(const Bnd_Size_F& size) { m_size = size; }
		inline void		setSize(const float width, const float height) { m_size = { width,height }; }
		inline void		setWidth(const float width) { m_size.width = width; }
		inline void		setHeight(const float height) { m_size.height = height; }
		inline Bnd_Size_F getSize() const { return m_size; }
		inline void		getSize(float& width, float& height) const { width = m_size.width; height = m_size.height; }
		inline float	getWidth() const { return m_size.width; }
		inline float	getHeight() const { return m_size.height; }

		void			setDstRect(const Bnd_Rect_F& dstRect);
		Bnd_Rect_F		getDstRect() const;

		void			setSrcRect(const Bnd_Rect_F* srcRect);
		Bnd_Rect_F		getSrcRect() const;					// REMINDER: might need to fix

		inline void		setOrigin(const origin& origin) { m_e_origin = origin; }
		inline void		setOrigin(const Vec2f& origin) { m_origin = { origin.x,origin.y }; }
		inline void		setOrigin(const float x, const float y) { m_origin = { x,y }; }

		inline Vec2f	getOrigin() const { return { m_origin.x,m_origin.y }; }

		inline void		setScale(const float scaleX, const float scaleY) { m_scale = { scaleX,scaleY }; }
		inline Vec2f	getScale() const { return m_scale; }

		inline void		setRotation(const float degree) { m_rotation = degree; }
		inline float	getRotation() const { return m_rotation; }


	};// class
};


#endif // DRAWABLE_H
