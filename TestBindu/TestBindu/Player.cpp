#include "Player.h"

Player::Player()
{
	moves = NONE;
	m_sprite.setCurrentRow(IDLE);
}

Player::~Player()
{
	
}

void Player::Init()
{
	m_sprite.LoadFromFile(L"Resource/fire-warrior2.png");
	m_sprite.LoadAnimationFile("Resource/fire-warrior-animation.txt");
	m_sprite.setPosition(20, 20);
	m_sprite.setSize(144, 80);
	m_sprite.setVelocity(20,20);
	m_sprite.setAnimationTimer(100);
	m_sprite.setAnimated(true);
}

void Player::Update(float dt)
{
	m_sprite.Update(dt);
	switch (moves)
	{
	case LEFT:
		m_sprite.setVelocityX(-40);
		m_sprite.setAnimation("dashing");
		break;
	case RIGHT:
		m_sprite.setVelocityX(40);
		m_sprite.setAnimation("running");
		break;
	case UP:
		m_sprite.setVelocityY(-40);
		m_sprite.setAnimation("walking");
		break;
	case DOWN:
		m_sprite.setVelocityY(40);
		break;
	case ATTACKING:
		m_sprite.setAnimation("fire_breath");
		break;
	case NONE:
	default:
		m_sprite.setVelocity(0, 0);
			m_sprite.setAnimation("idle");
		break;
	}
}

void Player::Move()
{
}

void Player::Animate()
{
	m_sprite.Animate();
}

void Player::Draw(BINDU::Graphics* graphics)
{
	m_sprite.Draw(graphics);
}

void Player::ProcessInput()
{
	moves = NONE;
	if (BINDU::Input::isKeyHold(BND_W))
		moves = UP;
	if (BINDU::Input::isKeyHold(BND_S))
		moves = DOWN;
	if (BINDU::Input::isKeyHold(BND_A))
		moves = LEFT;
	if (BINDU::Input::isKeyHold(BND_D))
		moves = RIGHT;
	if (BINDU::Input::isKeyHold(BND_J))
		moves = ATTACKING;
}
