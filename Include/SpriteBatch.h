#include "Bindu.h"
#pragma once


namespace BINDU
{
	class SpriteBatch : public SceneObject
	{
	private:
		ComPtr<ID2D1SpriteBatch>		m_spriteBatch;
		ComPtr<ID2D1DeviceContext3>		m_deviceContext;

		int								m_spriteCount{ 0 };

		float							m_coEff{ 0 };

		D2D1_MATRIX_3X2_F				m_cameraTransform{ D2D1::Matrix3x2F::Identity() };

	protected:


	public:
		SpriteBatch();
		~SpriteBatch();

		void 						Init();

		void						LoadSpriteSheet(const wchar_t* filename);

		void                        AddSprite(const Bnd_Rect_F& dstRect,const Bnd_Rect* srcRect, const BND_COLOR& color, float scale = 1, float rotation = 0);

		void						UpdateSprite(int index,int count, const Bnd_Rect_F* dstRect,const Bnd_Rect* srcRect, const BND_COLOR& newColor, float newScale = 1, float newRotation = 0);

		void						BatchAdd(int count, const Bnd_Rect_F& dstRect);

		inline int					getSpriteCount() const { return m_spriteBatch->GetSpriteCount(); }

		void						Clear() const;

		virtual void			    Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
		void						Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix, int index, int count = 1);

		virtual void				Update(float dt) override;
		virtual void				ProcessInput() override
		{
			
		}

		// Accessor/Mutator functions

		inline ID2D1SpriteBatch*    getSpriteBatch() const { return m_spriteBatch.Get(); }
		
	};
}// namespace