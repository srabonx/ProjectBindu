#include "Ball.h"

#include "Constants.h"
#include "GameManager.h"
#include "Input.h"

void Ball::Init()
{
	SetParticleEmitter();
}

void Ball::onLoadResource()
{
	BINDU::Texture bodyTex(RelativeResourcePath("Resource/images/balls.png").c_str());

	m_sprite.SetTexture(bodyTex);
	m_sprite.setPosition(this->m_position);
	m_sprite.setSize(this->m_size);
	m_sprite.setOrigin(this->getWidth() * 0.5f, this->getHeight() * 0.5f);

	m_animator.setFrameSize(8.f, 8.f);
	m_animator.setTotalColumn(2);
	m_animator.setTotalFrame(2);
	m_animator.setCurrentRow(0);
	m_animator.setCurrentFrame(1);

	BINDU::Texture tex(RelativeResourcePath("Resource/images/smoke.png").c_str());
	m_bodyEmitter->SetTexture(tex);
	m_collisionEmitter->SetTexture(tex);

	SceneObject::onLoadResource();
}

void Ball::onReleaseResource()
{
	SceneObject::onReleaseResource();
}

void Ball::Update(float dt)
{
	m_sprite.setPosition(this->m_position);
	m_sprite.setSize(this->m_size);

	if (m_bounce)
	{
		if (GameManager::getSfx())
			GameManager::getSoundSystem()->Play("ballsound", false, 0.3f);

		m_sprite.setScale(m_sprite.getScale().x + (2.0f - m_sprite.getScale().x) * 0.7f, m_sprite.getScale().y + (2.0f - m_sprite.getScale().y) * 0.7f);

		m_collisionEmitter->ChangeParticleVelocity({ m_velocity.x * 0.50f,-m_velocity.y * 0.10f });
		m_collisionEmitter->Generate();

	}else
	{
		//m_collisionEmitter->setActive(false);
		m_sprite.setScale(m_sprite.getScale().x + (1.0f - m_sprite.getScale().x) * 0.5f, m_sprite.getScale().y + (1.0f - m_sprite.getScale().y) * 0.5f);
	}

	if (m_sprite.getScale().x > 1.9f)
		m_bounce = false;
	

	if (m_rageMode)
	{
		m_animator.setCurrentFrame(2);
		m_bodyEmitter->ChangeParticleColor({ 255.f,0.f,0.f,50.f });
		
		if (m_timer >= 0)
		{
			--m_timer;
		}
		else
		{
			m_animator.setCurrentFrame(1);
			m_bodyEmitter->ChangeParticleColor({ 255,255,255,20 });
			m_rageMode = false;
			m_timer = 600;
		}
	}

	m_sprite.setRect(m_animator.getCurrentFrame());

	if (m_velocity.x > 300)
		m_velocity.x = 300;
	else if (m_velocity.x < -300)
		m_velocity.x = -300;
	if (m_velocity.y > 300)
		m_velocity.y = 300;
	else if (m_velocity.y < -300)
		m_velocity.y = -300;

	m_position += m_velocity * dt;


}

void Ball::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{
	m_sprite.Draw(graphics, cameraMatrix);
}

void Ball::ProcessInput()
{
	if (BINDU::Input::isKeyPressed(BND_V))
		m_rageMode = true;
	if (BINDU::Input::isKeyPressed(BND_L))
		m_bodyEmitter->ChangeParticleColor({ 0.f,0.f,255.f,50.f });
}

void Ball::Reset()
{
	m_stickyBall = false;
	m_bounce = false;

	m_timer = 0;
}


void Ball::SetParticleEmitter()
{
	std::unique_ptr<BINDU::ParticleEmitter> emitter = std::make_unique<BINDU::ParticleEmitter>();

	BINDU::ParticleProps props;

	props.size = { 10.f,10.f };
	props.velocity = { 0.f,10.f };
	props.startScale = 1.5f;
	props.endScale = 4.0f;
	props.startColor = { 255,255,255,5 };
	props.endColor = { 230,230,250,2 };
	

	props.minTimetoChangeColor = 1.5f;
	props.fadeOut = true;
	props.rotation = 0;
	props.rotationRate = 80;
	props.lifeTime = 2.0f;

	emitter->Init(props);
	emitter->setSize(0.f,0.f);
	emitter->onLoop(true);
	emitter->setDirection(95);
	emitter->setPosition(2.f,2.f);
	emitter->setMax(800);
	emitter->setSpread(180);
	emitter->setEmissionInterval(0);
	emitter->setEmissionRate(3);
	emitter->setActive(true);

	m_bodyEmitter = emitter.get();

	this->AddChild(std::move(emitter), "bodyemitter");

	std::unique_ptr<BINDU::ParticleEmitter> coliEmitter = std::make_unique<BINDU::ParticleEmitter>();

	props.size = { 10.f,10.f };
	props.velocity = { 0.f,0.f };
	props.startScale = 2.5f;
	props.endScale = 4.0f;
	props.startColor = { 255,255,255,10 };
	props.endColor = { 230,230,250,0 };


	props.minTimetoChangeColor = 2.0f;
	props.fadeOut = true;
	props.rotation = 0;
	props.rotationRate = 50;
	props.lifeTime = 2.0f;

	coliEmitter->Init(props);
	coliEmitter->setSize(m_size);
	coliEmitter->onLoop(false);
	coliEmitter->setDirection(95);
	coliEmitter->setPosition(0.0f, 0.0f);
	coliEmitter->setMax(1000);
	coliEmitter->setSpread(0);
	coliEmitter->setEmissionInterval(0);
	coliEmitter->setEmissionRate(25);
	coliEmitter->setActive(true);

	m_collisionEmitter = coliEmitter.get();

	AddChild(std::move(coliEmitter), "coliemitter");
}
