#include "Bindu.h"
#pragma once


namespace BINDU
{
	class SpriteBatch : protected Sprite
	{
	private:
		ComPtr<ID2D1SpriteBatch>		m_spriteBatch;
		ComPtr<ID2D1DeviceContext3>		m_deviceContxt;

		int								m_spriteCount{ 0 };

		std::vector<Vec2f>              m_velocities;

	protected:


	public:
		SpriteBatch();
		virtual ~SpriteBatch();

		void 						Init();
		void						LoadSpriteSheet(const wchar_t* filename);

		void                        SetSprite(const D2D1_RECT_F& dstRect, D2D1_RECT_U* srcRect, const BND_COLOR& color, float scale = 1, float rotation = 0, Vec2f velocity = {0,0});

		void						UpdateSprite(int index,const D2D1_RECT_F* dstRect,const BND_COLOR& newColor, float newScale = 1, float newRotation = 0, Vec2f newVelocity = { 0,0 });

		void						BatchAdd(int count, const D2D1_RECT_F& dstRect);

		inline int					GetSpriteCount() { return m_spriteBatch->GetSpriteCount(); }

		void						Clear();

		void                        StartSpriteBatch();
		
		void						DrawSelected(int index);

		virtual void			    Draw(Graphics* graphics) override;
		
		virtual void				Update(float dt) override;

		// Accessor/Mutator functions

		inline ID2D1SpriteBatch*    getSpriteBatch() { return m_spriteBatch.Get(); }

		void						setSpriteCount(int value);
		inline int					getSpriteCount() { return static_cast<int>(m_spriteBatch->GetSpriteCount()); }

		
	};
}// namespace