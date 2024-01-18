#include "Bindu.h"
#pragma once


namespace BINDU
{
	class SpriteBatch : public Drawable
	{
	private:
		ComPtr<ID2D1SpriteBatch>		m_spriteBatch;
		ComPtr<ID2D1DeviceContext3>		m_deviceContext;

		int								m_spriteCount{ 0 };

		float							m_coEff{ 0 };

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

		virtual void			    Draw(Graphics* graphics) override;
		void						Draw(Graphics* graphics, int index, int count = 1) const;

		void						Update(float dt);

		// Accessor/Mutator functions

		inline ID2D1SpriteBatch*    getSpriteBatch() const { return m_spriteBatch.Get(); }
		
	};
}// namespace