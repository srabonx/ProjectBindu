
#include <Bindu.h>
#include "GameManager.h"

//class Ball: public BINDU::SceneObject
//{
//private:
//	BINDU::Sprite m_sprite;
//	BINDU::Bnd_Rect_F m_srcRect;
//
//	BINDU::Vec2f m_velocity;
//
//public:
//
//	Ball() = default;
//	~Ball() = default;
//
//	void LoadResources()
//	{
//		m_sprite.LoadSpriteFromFile(L"BrickBreaker/Resource/paddles_and_balls.png");
//		m_sprite.setPosition(this->m_position);
//		m_sprite.setSize(this->m_size);
//
//		m_srcRect = { 127,8,9,9 };
//
//		m_velocity= { 50,50 };
//
//	}
//
//	void Update(float dt) override
//	{
//		m_sprite.setPosition(m_position);
//
//		m_position.x += m_velocity.x * dt;
//		m_position.y += m_velocity.y * dt;
//	}
//
//	void ProcessInput() override
//	{
//		
//	}
//
//	void Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override
//	{
//		m_sprite.Draw(m_srcRect, graphics, cameraMatrix);
//	}
//
//	void setVelocity (const BINDU::Vec2f& velocity)
//	{
//		m_velocity = velocity;
//	}
//
//	BINDU::Vec2f getVelocity() const { return m_velocity; }
//
//};
//
//class Paddle:public BINDU::SceneObject
//{
//private:
//	BINDU::Sprite m_sprite;
//	BINDU::Bnd_Rect_F m_srcRect;
//
//	bool m_moveLeft{};
//	bool m_moveRight{};
//
//	float  m_velocity{};
//
//public:
//
//	Paddle() = default;
//	~Paddle() = default;
//
//	void LoadResources()
//	{
//		m_sprite.LoadSpriteFromFile(L"BrickBreaker/Resource/images/paddles.png");
//		m_sprite.setPosition(this->m_position);
//		m_sprite.setSize(this->m_size);
//
//		m_srcRect = { 0,0,48,15.5 };
//
//		m_velocity = 200.f;
//	}
//
//	void Update(float dt) override
//	{
//		m_sprite.setPosition(m_position);
//
//
//		if(m_moveLeft)
//		{
//			m_velocity = -m_velocity;
//			m_position.x += m_velocity * dt;
//		}
//
//		if(m_moveRight)
//		{
//			m_velocity = 200;
//			m_position.x += m_velocity * dt;
//		}
//				
//	}
//
//	void ProcessInput() override
//	{
//		m_moveLeft = false;
//		m_moveRight = false;
//
//		if (BINDU::Input::isKeyHold(BND_A))
//			m_moveLeft = true;
//		if (BINDU::Input::isKeyHold(BND_D))
//			m_moveRight = true;
//
//	}
//
//	void Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override
//	{
//		m_sprite.Draw(m_srcRect,graphics, cameraMatrix);
//	}
//
//	void setVelocity(float velocity)
//	{
//		m_velocity = velocity;
//	}
//
//	float getVelocity() const { return m_velocity; }
//
//};
//
//class LevelManager
//{
//private:
//	std::unique_ptr<BINDU::Sprite> m_background;
//	std::unique_ptr<BINDU::Sprite> m_boundary;
//
//	std::unique_ptr<Paddle> m_paddle;
//
//	std::unique_ptr<Ball> m_ball;
//
//	std::unique_ptr<BINDU::Scene>  m_scene1;
//
//	std::unique_ptr<BINDU::Layer>  m_fgLayer;
//	std::unique_ptr<BINDU::Layer>  m_bgLayer;
//
//	Ball* pBall = nullptr;
//	Paddle* pPaddle = nullptr;
//	BINDU::Sprite* pBoundary = nullptr;
//
//public:
//
//	LevelManager() = default;
//	~LevelManager() = default;
//
//	void Init()
//	{
//		m_scene1 = std::make_unique<BINDU::Scene>();
//
//		m_fgLayer = std::make_unique<BINDU::Layer>();
//		m_bgLayer = std::make_unique<BINDU::Layer>();
//
//		m_background = std::make_unique<BINDU::Sprite>();
//		m_boundary = std::make_unique<BINDU::Sprite>();
//
//		m_paddle = std::make_unique<Paddle>();
//		m_ball = std::make_unique<Ball>();
//	}
//
//	void LoadResources()
//	{
//		m_background->LoadSpriteFromFile(L"BrickBreaker/Resource/images/background.png");
//		m_background->setPosition(0.f, 0.f);
//		m_background->setSize(g_engine->getWindowWidth(), g_engine->getWindowHeight());
//
//		m_background->setActive(true);
//		m_background->setCollidable(false);
//
//		m_boundary->LoadSpriteFromFile(L"BrickBreaker/Resource/images/borders.png");
//		m_boundary->setPosition(0, 50);
//		m_boundary->setSize(450, 500);
//
//		m_boundary->setActive(true);
//		m_boundary->setCollidable(false);
//
//
//		m_paddle->setPosition(200, 500);
//		m_paddle->setSize(100, 20);
//		m_paddle->LoadResources();
//
//		m_paddle->setActive(true);
//		m_paddle->setColliderOffset(0, 0, 45, 10);
//		m_paddle->setCollideAble(true);
//
//		m_ball->setPosition(200, 440);
//		m_ball->setSize(20, 20);
//		m_ball->LoadResources();
//		m_ball->setVelocity({ -100, -400 });
//
//		m_ball->setActive(true);
//		m_ball->setCollideAble(true);
//
//		m_bgLayer->AddObject(std::move(m_background), "bgImage");
//		m_bgLayer->setActive(true);
//
//		m_fgLayer->AddObject(std::move(m_boundary), "boundary");
//		m_fgLayer->AddObject(std::move(m_paddle), "paddle");
//		m_fgLayer->AddObject(std::move(m_ball), "ball");
//		m_fgLayer->showDebug(true);
//		m_fgLayer->setActive(true);
//
//
//		m_scene1->AddLayer(std::move(m_bgLayer), "bgLayer");
//		m_scene1->AddLayer(std::move(m_fgLayer), "fgLayer");
//
//		m_scene1->setActive(true);
//
//
//		pBoundary = dynamic_cast<BINDU::Sprite*>(m_scene1->getLayer("fgLayer")->getObject("boundary"));
//		pBall = dynamic_cast<Ball*>(m_scene1->getLayer("fgLayer")->getObject("ball"));
//		pPaddle = dynamic_cast<Paddle*>(m_scene1->getLayer("fgLayer")->getObject("paddle"));
//
//	}
//
//	void Update(float dt)
//	{
//
//		m_scene1->Update(dt);
//
//	
//		pPaddle->setVelocity(200);
//
//		if (pPaddle->getPosition().x <= pBoundary->getPosition().x + 20)
//			pPaddle->setX(pBoundary->getPosition().x + 20);
//		if (pPaddle->getPosition().x + pPaddle->getSize().width - 25 >= pBoundary->getSize().width)
//			pPaddle->setX(pBoundary->getSize().width - (pPaddle->getSize().width - 25));
//
//
//		if (pBall->getPosition().x <= pBoundary->getPosition().x + 20)
//			pBall->setVelocity({ pBall->getVelocity().x * -1,pBall->getVelocity().y});
//		if (pBall->getPosition().x + pBall->getSize().width >= pBoundary->getSize().width - 20)
//			pBall->setVelocity({ pBall->getVelocity().x * -1,pBall->getVelocity().y });
//		if (pBall->getPosition().y <= pBoundary->getPosition().y + 20)
//			pBall->setVelocity({ pBall->getVelocity().x ,pBall->getVelocity().y * -1 });
//
//
//
//	}
//
//	void ProcessInput()
//	{
//		m_scene1->ProcessInput();
//	}
//
//	void Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
//	{
//		/*m_background.Draw(graphics, cameraMatrix);
//		m_boundary.Draw(graphics, cameraMatrix);
//
//		m_paddle.Draw(graphics, cameraMatrix);
//		m_ball.Draw(graphics, cameraMatrix);*/
//		m_scene1->Draw(graphics, cameraMatrix);
//	}
//
//	void CheckCollision(BINDU::SceneObject* a, BINDU::SceneObject* b)
//	{
//		if(pPaddle == dynamic_cast<Paddle*>(a))
//		{
//			if(pBall == dynamic_cast<Ball*>(b))
//			{
//				pBall->setVelocity({pPaddle->getVelocity(),-300});
//			}
//		}
//	}
//
//};
//
//
//class BrickBreaker
//{
//private:
//	LevelManager m_levelManager;
//
//public:
//
//	void Preload()
//	{
//		BINDU::EngineProperties props;
//		props.versionNotice = true;
//		props.windowWidth = 640.f;
//		props.windowHeight = 600.f;
//		props.windowTitle = "BrickBreaker";
//
//		g_engine->setEngineProperties(props);
//
//	}
//
//	void Init()
//	{
//		m_levelManager.Init();
//
//		m_levelManager.LoadResources();
//	}
//
//	void Update(float dt)
//	{
//		m_levelManager.Update(dt);
//	}
//
//	void ProcessInput()
//	{
//		m_levelManager.ProcessInput();
//	}
//
//	void Draw(BINDU::Graphics* graphics)
//	{
//		m_levelManager.Draw(graphics, D2D1::Matrix3x2F::Identity());
//	}
//
//	void CheckCollision(BINDU::SceneObject* a, BINDU::SceneObject* b)
//	{
//		m_levelManager.CheckCollision(a, b);
//	}
//
//};

bool BINDU::e_gameOver = false;

GameManager* bb;

bool BINDU::game_preload()
{
	bb = new GameManager();
	bb->Preload();
	return true;
}

bool BINDU::game_init()
{
	bb->Init();
	bb->onLoadResource();
	return true;
}

void BINDU::game_end()
{
	delete bb;
}

void BINDU::game_update(float dt)
{
	bb->Update(dt);
}

void BINDU::game_render2d(Graphics* graphics)
{
	bb->Draw(graphics);
}

void BINDU::game_processInputs()
{
	//bb->ProcessInput();
}

void BINDU::game_collision(SceneObject* a, SceneObject* b)
{
	//bb->CheckCollision(a, b);
}

