#pragma once
#include "SceneObject.h"
#include "Texture.h"

namespace BINDU
{

	class Sprite :public SceneObject
	{

	private:

		D2D1_RECT_F		m_srcRect{};

		float			m_opacity{1};

	public:
		Sprite();
		Sprite(const Texture& texture, const Bnd_Rect_F& rect);

		~Sprite() = default;

		void				SetTexture(const Texture& texture);

		void				Update(float dt) override;
		void				Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
		void				Draw(Bnd_Rect_F srcRect, Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix);
		virtual void		ProcessInput() override
		{}

		void				setRect(const Bnd_Rect_F& rect);
		Bnd_Rect_F			getRect() const;

		inline void			setOpacity(const float opacity) { m_opacity = opacity / 255.f; }
		inline float		getOpacity() const { return m_opacity * 255; }


	};// Class

};// Namespace