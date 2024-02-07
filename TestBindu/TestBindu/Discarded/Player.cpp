#include "Player.h"

Player::Player()
{
	moves = NONE;
	m_animator.setCurrentRow(IDLE);
}

Player::~Player()
{
	
}

void Player::Init()
{
	
	m_sprite.LoadSpriteFromFile(L"Resource/fire-warrior2.png");
	m_animator.LoadAnimationFile("Resource/fire-warrior-animation.txt");
	m_sprite.setPosition(m_position);
	m_size = { 144,80 };
	m_sprite.setSize(144, 80);
	m_velocity = { 200.f,200.f };
	m_animator.setFrameTime(100);
}

void Player::Update(float dt)
{
	m_animator.Animate(m_srcRect);
	m_sprite.setY(m_position.y);


	switch (moves)
	{
	case LEFT:
		m_position.x -= m_velocity.x * dt;
		m_sprite.setScale(-1, 1);
		m_sprite.setX(m_position.x - 30);
		m_animator.setAnimation("running");
		break;
	case RIGHT:
		m_position.x += m_velocity.x * dt;
		m_sprite.setScale(1, 1);
		m_sprite.setX(m_position.x);
		m_animator.setAnimation("running");
		break;
	case UP:
		m_position.y -= m_velocity.y * dt;
		m_animator.setAnimation("walking");
		break;
	case DOWN:
		m_position.y += m_velocity.y * dt;
		m_animator.setAnimation("walking");
		break;
	case ATTACKING:
		m_animator.setAnimation("fire_breath");
		break;
	case NONE:
	default:
		//m_velocity = { 0,0 };
		m_animator.setAnimation("idle");
		break;
	}
	
}

void Player::Move()
{
}

void Player::Animate()
{
	
}

void Player::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{
	UpdateTransforms();
	graphics->getRenderTarget()->SetTransform(m_transforms * cameraMatrix);
	graphics->getRenderTarget()->DrawRectangle({ m_position.x,m_position.y,(m_position.x + m_size.width) ,(m_position.y + m_size.height)}, graphics->getSolidColorBrush());
	m_sprite.Draw(m_srcRect,graphics,cameraMatrix);
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