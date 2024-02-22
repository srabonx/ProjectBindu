#pragma once
#include <Bindu.h>

#include "Animator.h"
#include "ParticleEmitter.h"
#include "Sprite.h"

enum class PaddleType
{
	SMALL,
	MID,
	LARGE,
	LASER,
	MAGNETIC
};

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

	PaddleType			m_paddleType = PaddleType::MID;


	BINDU::Texture		m_bulletTex;
	std::vector<std::unique_ptr<BINDU::Sprite>> m_bullets;

	BINDU::ParticleEmitter* m_muzzleFlashEmitter;


	int					m_counter{600};

private:

	void	SetUpParticleEmitter();
	void	FireBullet();

public:

	Paddle() = default;
	~Paddle() = default;

	void	Init();

	void    onLoadResource() override;

	void	onReleaseResource() override;

	void    Update(float dt) override;

	void	Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;

	void    ProcessInput() override;

	void    CheckCollision( SceneObject& sceneObject) override;

	inline BINDU::Vec2f getVelocity() { return m_velocity; }
	inline void			setVelocity(const BINDU::Vec2f& velocity) { m_velocity = velocity;}

	inline void	setPaddleType(const PaddleType& paddleType) { m_paddleType = paddleType; }
	inline PaddleType getPaddleType() const { return m_paddleType; }

	inline std::vector<std::unique_ptr<BINDU::Sprite>>& getBullets() { return m_bullets; }

};

