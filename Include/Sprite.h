#include "Bindu.h"
#pragma once

namespace BINDU
{

	class Sprite :public SceneObject
	{

	private:

		enum CollisionType
		{
			COLLISION_RECT,
			COLLISION_CIRC,
			COLLISION_NONE,
		};

	private:

		bool							   m_collideable;

		CollisionType					   m_collisionType;


		bool							   m_doesScale{ false };
		Timer							   m_scaleTimer;
		int                                m_scaleTime{ 0 };
		float                              m_scaleDelta{ 0.0f };
		float							   m_minScale{ 0.0f };
		float                              m_maxScale{ 0.0f };

		bool							   m_doesRotate;
		Timer							   m_rotationTimer;
		int								   m_rotationTime;
		float							   m_rotationDelta;

		
	public:
		Sprite();
		~Sprite() = default;

		bool				LoadSpriteFromFile(const wchar_t* filename) { return LoadFromFile(filename); }

		virtual void		Update(float dt) override;
		virtual void		Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
		void				Draw(Bnd_Rect_F srcRect, Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix);
		virtual void		ProcessInput() override
		{}
		virtual void UpdateTransforms() override;

		inline void		    doesScale(bool value) { m_doesScale = value; }
		inline void         setScaleTimer(int ms) { m_scaleTime = ms; }
		inline void			setScaleDelta(float value) { m_scaleDelta = value; }
		inline void         setScaleRatio(float min, float max) { m_minScale = min, m_maxScale = max; }

		inline void         doesRotate(bool value) { m_doesRotate = value; }    
		inline void			setRotationTimer(int ms) { m_rotationTime = ms; }

		inline bool			isCollidable() const { return m_collideable; }
		inline void			setCollidable(int value) { m_collideable = value; }

		inline CollisionType getCollisionType() const { return m_collisionType; }
		inline void			 setCollisionMethod(CollisionType type) { m_collisionType = type; }
		
	};// Class

};// Namespace