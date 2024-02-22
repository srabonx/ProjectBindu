#ifndef BALL_H
#define BALL_H
#include <SceneObject.h>

#include "Animator.h"
#include "ParticleEmitter.h"
#include "Sprite.h"

class Ball : public BINDU::SceneObject
{
private:
	BINDU::Sprite m_sprite;

	BINDU::Vec2f  m_velocity{};

	BINDU::Animator m_animator;

	BINDU::Bnd_Rect_F m_srcRect{};

	BINDU::ParticleEmitter*	m_bodyEmitter;
	BINDU::ParticleEmitter* m_collisionEmitter;

	bool			m_rageMode{ false };

	bool			m_stickyBall{ false };

	bool			m_bounce{ false };

	float			m_delta{};

	int				m_timer{ 600 };



private:

	void  SetParticleEmitter();

public:

	Ball() = default;
	Ball(const BINDU::Vec2f& position, const BINDU::Bnd_Size_F& size, const BINDU::Vec2f& velocity): m_velocity(velocity)
	{
		m_position = position;
		m_size = size;
	}
	~Ball() = default;

	void	Init();
	void	onLoadResource() override;
	void	onReleaseResource() override;

	void	Update(float dt) override;
	void	Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
	void	ProcessInput() override;

	void	Reset();

	inline void setVelocity(const BINDU::Vec2f& velocity) { m_velocity = velocity; }
	inline BINDU::Vec2f getVelocity() const { return m_velocity; }

	inline bool isRageMode() const { return m_rageMode; }
	inline void setRageMode(bool value) { m_rageMode = value; }

	inline bool isStickyBall() const { return m_stickyBall; }
	inline void setStickyBall(bool value) { m_stickyBall = value; }

	inline void setDelta(const float delta) { m_delta = delta; }
	inline float getDelta() const { return  m_delta; }

	inline void setBounce(const float bounce) { m_bounce = bounce; }

	inline BINDU::ParticleEmitter* getBodyEmitter() { return m_bodyEmitter; }

};

#endif // BALL_H
