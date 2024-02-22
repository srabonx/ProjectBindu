#include "Paddle.h"

#include "Ball.h"
#include "Input.h"
#include "Constants.h"
#include "GameManager.h"


void Paddle::Init()
{
	SetUpParticleEmitter();

}

void Paddle::onLoadResource()
{
	m_bodyTexture.LoadFromFile(RelativeResourcePath("Resource/images/paddles.png").c_str());
	m_bodySprite.SetTexture(m_bodyTexture);
	m_bodySprite.setPosition(m_position);
	m_bodySprite.setSize(m_size);

	m_bulletTex.LoadFromFile(RelativeResourcePath("Resource/images/laserbeam.png").c_str());

	m_animator.setFrameSize(48.f, 15.5f);
	m_animator.setTotalColumn(1);
	m_animator.setTotalFrame(4);
	m_animator.setCurrentFrame(2);

	m_velocity = { 200.f,200.f };

	BINDU::Texture tex(RelativeResourcePath("Resource/images/muzzle_04.png").c_str());
	m_muzzleFlashEmitter->SetTexture(tex);

	SceneObject::onLoadResource();
}

void Paddle::onReleaseResource()
{
	SceneObject::onReleaseResource();
}

void Paddle::Update(float dt)
{
//	UpdateTransform();

	m_bodySprite.setPosition(m_position);
	m_bodySprite.setSize(m_size);
	m_srcRect = m_animator.getCurrentFrame();

	setY(getY() + (600.f - getY()) * 0.4f);

	m_position += m_velocity * dt;

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;

	if (m_moveLeft)
		m_velocity.x = -300.f;
	if (m_moveRight)
		m_velocity.x = +300.f;
	if (m_moveDown)
		m_velocity.y = +300.f;
	if (m_moveUp)
		m_velocity.y = -300.f;

	switch (m_paddleType)
	{
	case PaddleType::SMALL:
		m_animator.setCurrentFrame(1);
		setCollider({ 0.f,0.f,m_size.width * 0.5f,m_size.height });
		break;
	case PaddleType::MID:
		m_animator.setCurrentFrame(2);
		setCollider({ 0.f,0.f,m_size.width * 0.67f,m_size.height });
		break;
	case PaddleType::LARGE:
		m_animator.setCurrentFrame(3);
		setCollider({ 0.f,0.f,m_size.width,m_size.height });
		break;
	case PaddleType::LASER:
		m_animator.setCurrentFrame(4);
		setCollider({ 0.f,0.f,m_size.width * 0.67f,m_size.height });
		break;
	case PaddleType::MAGNETIC:
		m_animator.setCurrentFrame(3);
		setCollider({ 0.f,0.f,m_size.width,m_size.height });

		if (m_counter >= 0)
			m_counter--;
		else
		{
			m_paddleType = PaddleType::LARGE;
			m_counter = 600;
		}
		break;

	default:
		break;
	}

}

void Paddle::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{

	m_bodySprite.Draw(m_srcRect, graphics, cameraMatrix);

	for (const auto& m : m_bullets)
	{
		if (m)
			m->Draw(graphics, cameraMatrix);
	}
}

void Paddle::ProcessInput()
{
	if (BINDU::Input::isKeyPressed(BND_M))
		m_paddleType = PaddleType::MAGNETIC;

	m_moveLeft = false;
	m_moveRight = false;
	m_moveDown = false;
	m_moveUp = false;

	if (BINDU::Input::isKeyHold(BND_A))
		m_moveLeft = true;
	if (BINDU::Input::isKeyHold(BND_D))
		m_moveRight = true;

	if (m_paddleType == PaddleType::LASER && BINDU::Input::isKeyPressed(BND_SPACE) && m_bullets.size() < 2)
	{
		if (GameManager::getSfx())
			GameManager::getSoundSystem()->Play("lasersound", false, 1.0f);

		FireBullet();
		m_muzzleFlashEmitter->setPosition((getRectCollider().w * 0.5f) - 25.f, -50.f);
		m_muzzleFlashEmitter->Generate();
	}

	if (BINDU::Input::isKeyPressed(BND_N))
	{
		if (GameManager::getSfx())
			GameManager::getSoundSystem()->Play("lasersound", false, 1.0f);

		m_muzzleFlashEmitter->setPosition((getRectCollider().w * 0.5f ) - 25.f, -50.f);
		m_muzzleFlashEmitter->Generate();
	}

	//if (BINDU::Input::isKeyHold(BND_S))
	//	m_moveDown = true;
	//if (BINDU::Input::isKeyHold(BND_W))
	//	m_moveUp = true;

}

void Paddle::CheckCollision(SceneObject& sceneObject)
{

}

void Paddle::SetUpParticleEmitter()
{
	std::unique_ptr<BINDU::ParticleEmitter> bulletEmitter = std::make_unique<BINDU::ParticleEmitter>();

	BINDU::ParticleProps props;

	props.size = { 50.f,50.f };
	props.velocity = { 0.f,0.f };
	props.startScale = 1.5f;
	props.endScale = 1.0f;
	props.startColor = { 0,255,0,250 };
	props.endColor = { 0,84,0,4000 };


	props.minTimetoChangeColor = 0.0f;
	props.fadeOut = true;
	props.rotation = 0;
	props.rotationRate = 0;
	props.lifeTime = 1.f;

	bulletEmitter->Init(props);
	bulletEmitter->setSize(0.f,10.f);
	bulletEmitter->onLoop(false);
	bulletEmitter->setDirection(0);
	bulletEmitter->setPosition(getRectCollider().w * 0.5f, 0.0f);
	bulletEmitter->setMax(1);
	bulletEmitter->setSpread(0);
	bulletEmitter->setEmissionInterval(0);
	bulletEmitter->setEmissionRate(1);
	bulletEmitter->setActive(true);

	m_muzzleFlashEmitter = bulletEmitter.get();
	AddChild(std::move(bulletEmitter), "bulletemitter");
}

void Paddle::FireBullet()
{
		std::unique_ptr<BINDU::Sprite> bullet = std::make_unique<BINDU::Sprite>();
		const BINDU::Bnd_Size_F size = { 8.f,16.f };

		bullet->setPosition(getX() + (getRectCollider().w * 0.5f) - (size.width * 0.5f), getY() - 20.f);
		bullet->setSize(size);
		bullet->scaleY(0.0f);
		bullet->SetTexture(m_bulletTex);

		setY(getY() + 10.f);

		m_bullets.push_back(std::move(bullet));

}
