#include "Bindu.h"
#pragma once

namespace BINDU
{
	enum CollisionType
	{
		COLLISION_RECT,
		COLLISION_CIRC,
		COLLISION_NONE,
	};

	struct AnimDetails
	{
		int index;
		int framecount;
	};

	class Sprite :public Entity
	{
	private:

		bool							   m_imageLoaded;
		int								   m_state;
		int								   m_direction;
		bool							   m_animated;
		D2D1_RECT_F						   srcRect;

		bool							   m_collideable;
		CollisionType					   m_collisionType;

		std::map<std::string, AnimDetails> m_animationMap;

	protected:

		ComPtr<ID2D1Bitmap>				   m_bitmap;

		D2D1_SIZE_F                        m_bitmapSize;

		Vec2f							   m_position;
		Vec2f							   m_velocity;

		float							   m_width;
		float							   m_height;

		float							   m_scale;
		float							   m_rotation;
		float							   m_translateX;
		float							   m_translateY;

		D2D1_POINT_2F					   m_center;
		D2D1::Matrix3x2F				   m_translateMatrix;
		D2D1::Matrix3x2F				   m_rotationMatrix;
		D2D1::Matrix3x2F				   m_scalingMatrix;

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

		int								   m_totalFrame;
		int								   m_totalColumn;
		int								   m_currentFrame;
		int								   m_currentRow;
		int								   m_timeCount;

		float							   m_animFrameWidth;
		float							   m_animFrameHeight;
		Timer							   m_animTimer;
		int							       m_animDir;

		int								   m_moveStart;
		int								   m_moveTimer;


		void updateTransform();
	public:
		Sprite();
		virtual ~Sprite();

		bool				LoadFromFile(const wchar_t* filename);
		bool				LoadAnimationFile(const char* filename);
		void				SetBitmap(ID2D1Bitmap* bitmap);
		virtual void		Update(float dt);
		virtual void		Draw(Graphics* graphics);
		void				DrawSub(D2D1_RECT_F rect, Graphics* graphics);
		virtual void		Animate();
		virtual void		Move();

		// Screen position

		inline Vec2f		getPosition() const { return m_position; }
		inline void			setPosition(const Vec2f& position) { m_position = position; }
		inline void			setPosition(float x, float y) { setPosition({ x,y }); }

		inline void			setX(float value) { m_position.x = value; }
		inline void			setY(float value) { m_position.y = value; }

		inline float		getX() const { return m_position.getX(); }
		inline float		getY() const { return m_position.getY(); }

		// Movement velocity
		inline Vec2f		getVelocity() const { return m_velocity; }
		inline void			setVelocity(const Vec2f& velocity) { m_velocity = velocity; }
		inline void			setVelocity(float xVel, float yVel) { setVelocity({ xVel,yVel }); }
		inline void			setVelocityX(float xVel) { m_velocity.x = xVel; }
		inline void			setVelocityY(float yVel) { m_velocity.y = yVel; }

		// Image size
		inline void			setSize(float width, float height) { m_width = width; m_height = height; }

		inline void			setWidth(float width) { m_width = width; }
		inline void			setHeight(float height) { m_height = height; }

		inline float		getWidth() const { return m_width; }
		inline float		getHeight() const { return m_height; }

		inline void			setScale(float scale) { m_scale = scale; }
		inline float		getScale() const { return m_scale; }

		inline void		    doesScale(bool value) { m_doesScale = value; }
		inline void         setScaleTimer(int ms) { m_scaleTime = ms; }
		inline void			setScaleDelta(float value) { m_scaleDelta = value; }
		inline void         setScaleRatio(float min, float max) { m_minScale = min, m_maxScale = max; }

		inline void			setRotation(float degree) { m_rotation = degree; m_rotationDelta = m_rotation; }
		inline float		getRotation() const { return m_rotation; }

		inline void         doesRotate(bool value) { m_doesRotate = value; }    
		inline void			setRotationTimer(int ms) { m_rotationTime = ms; }

		inline void			translateX(float value) { m_translateX = value; }
		inline void 		translateY(float value) { m_translateY = value; }
		inline void			translate(float xTranslate, float yTranslate) { m_translateX = xTranslate; m_translateY = yTranslate; }

		//inline ID2D1Bitmap* GetBitmap() { return m_bitmap; }
		//inline BitmapLoader& GetBitmapLoader() { return bmpLoader; }

		inline int			getMoveTimer() const { return m_moveTimer; }
		inline void			setMoveTimer(int value) { m_moveTimer = value; }

		inline void			setTotalFrame(int value) { m_totalFrame = value; }
		inline int			getTotalFrame() const { return m_totalFrame; }

		inline void			setTotalColumn(int value) { m_totalColumn = value; }
		inline int			getTotalColumn() const { return m_totalColumn; }

		inline void			setCurrentRow(int value) { m_currentRow = value; }
		inline int			getCurrentRow() const { return m_currentRow; }

		inline void			setAnimated(bool value) { m_animated = value; }
		inline bool			getAnimated() { return m_animated; }

		inline void			setAnimationFrameWidth(float width) { m_animFrameWidth = width; }
		inline float		getAnimationFrameWidth() const { return m_animFrameWidth; }

		inline void			setAnimationFrameHeight(float height) { m_animFrameHeight = height; }
		inline float		getAnimationFrameHeight() const { return m_animFrameHeight; }

		inline void			setCurrentFrame(int value) { m_currentFrame = value; }
		inline int			getCurrentFrame() const { return m_currentFrame; }

		inline void			setAnimationTimer(int ms) { m_timeCount = ms; }
		void				setAnimation(const char* animation);

		inline int			getAnimationDirection() const { return m_animDir; }
		inline void			setAnimationDirection(int value) { m_animDir = value; }

		inline bool			isCollidable() const { return m_collideable; }
		inline void			setCollidable(int value) { m_collideable = value; }

		inline CollisionType getCollisionType() const { return m_collisionType; }
		inline void			 setCollisionMethod(CollisionType type) { m_collisionType = type; }
		
	};// Class

};// Namespace