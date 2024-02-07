#include "Paddle.h"

#include "Input.h"
#include "Constants.h"

void Paddle::Init()
{

}

void Paddle::onLoadResource()
{
	m_bodyTexture.LoadFromFile(RelativeResourcePath("Resource/images/paddles.png").c_str());
	m_bodySprite.SetTexture(m_bodyTexture);
	m_bodySprite.setPosition(m_position);
	m_bodySprite.setSize(m_size);

	m_animator.setFrameSize(48.f, 16.f);
	m_animator.setTotalColumn(1);
	m_animator.setTotalFrame(4);
	m_animator.setCurrentFrame(3);

	m_velocity = { 200.f,200.f };
}

void Paddle::Update(float dt)
{
	UpdateTransform();

	m_bodySprite.setPosition(m_position);
	m_bodySprite.setSize(m_size);
	m_srcRect = m_animator.getCurrentFrame();

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

}

void Paddle::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{
	m_bodySprite.Draw(m_srcRect, graphics, cameraMatrix);
}

void Paddle::ProcessInput()
{
	if (BINDU::Input::isKeyPressed(BND_H))
		m_animator.setCurrentFrame(1);
	if (BINDU::Input::isKeyPressed(BND_J))
		m_animator.setCurrentFrame(2);
	if (BINDU::Input::isKeyPressed(BND_K))
		m_animator.setCurrentFrame(3);
	if (BINDU::Input::isKeyPressed(BND_L))
		m_animator.setCurrentFrame(4);

	m_moveLeft = false;
	m_moveRight = false;
	m_moveDown = false;
	m_moveUp = false;

	if (BINDU::Input::isKeyHold(BND_A))
		m_moveLeft = true;
	if (BINDU::Input::isKeyHold(BND_D))
		m_moveRight = true;
	//if (BINDU::Input::isKeyHold(BND_S))
	//	m_moveDown = true;
	//if (BINDU::Input::isKeyHold(BND_W))
	//	m_moveUp = true;

}

void Paddle::CheckCollision(const SceneObject& sceneObject)
{
	if (sceneObject.getGuid() == "spr")
		setPosition(100, 100);
}
