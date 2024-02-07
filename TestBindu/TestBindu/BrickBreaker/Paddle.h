#pragma once
#include <Bindu.h>

#include "Animator.h"
#include "Sprite.h"

class Paddle :public BINDU::SceneObject
{
private:
	BINDU::Texture		m_bodyTexture;
	BINDU::Sprite		m_bodySprite;

	BINDU::Vec2f		m_velocity;

	bool				m_moveLeft{};
	bool				m_moveRight{};
	bool				m_moveUp{};
	bool				m_moveDown{};

	BINDU::Animator		m_animator{};
	BINDU::Bnd_Rect_F   m_srcRect{};

public:

	Paddle() = default;
	~Paddle() = default;

	void	Init();

	void    onLoadResource();

	void    Update(float dt) override;

	void	Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;

	void    ProcessInput() override;

	void    CheckCollision(const SceneObject& sceneObject) override;

	inline BINDU::Vec2f getVelocity() { return m_velocity; }
	inline void			setVelocity(const BINDU::Vec2f& velocity) { m_velocity = velocity;}


};

