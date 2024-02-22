#include "MainScene.h"

#include <algorithm>

#include "CollisionHandler.h"
#include "GameManager.h"
#include "Input.h"
#include "Random.h"


void MainScene::Init(const BINDU::Bnd_Size& windSize)
{
	m_windSize = windSize;

	m_font.Init();

	m_camera.setViewSize(0.f, 0.f);
	m_camera.setBottomRight(800.f, 1400.f);
}

void MainScene::onLoadResource()
{

	GameManager::getSoundSystem()->Load("ballsound", RelativeResourcePath("Resource/sounds/pong.wav").c_str());
	GameManager::getSoundSystem()->Load("lasersound", RelativeResourcePath("Resource/sounds/laser7.mp3").c_str());
	GameManager::getSoundSystem()->Load("levelcomplete", RelativeResourcePath("Resource/sounds/level-complete.ogg").c_str());
	GameManager::getSoundSystem()->Load("lifelost", RelativeResourcePath("Resource/sounds/life-lost.ogg").c_str());
	GameManager::getSoundSystem()->Load("powerup", RelativeResourcePath("Resource/sounds/power-up.ogg").c_str());
	GameManager::getSoundSystem()->Load("brickfall", RelativeResourcePath("Resource/sounds/highDown.mp3").c_str());


	BINDU::Texture bgTexture(RelativeResourcePath("Resource/images/background.png").c_str());
	BINDU::Texture boundaryTexture(RelativeResourcePath("Resource/images/borders.png").c_str());

	m_powerUpTex.LoadFromFile(RelativeResourcePath("Resource/images/power-ups.png").c_str());
	m_healthTex.LoadFromFile(RelativeResourcePath("Resource/images/hud-life.png").c_str());

	m_powerUpAnimator.setTotalColumn(4);
	m_powerUpAnimator.setTotalFrame(8);
	m_powerUpAnimator.setFrameSize(20.f, 12.f);

	std::unique_ptr<BINDU::Sprite> bgImg = std::make_unique<BINDU::Sprite>();
	bgImg->SetTexture(bgTexture);
	bgImg->setPosition(0.f, 0.f);
	bgImg->setSize(static_cast<float>(m_windSize.width), static_cast<float>(m_windSize.height));
	bgImg->setActive(true);
	m_backgroundImg = bgImg.get();

	std::unique_ptr<BINDU::Sprite> bndryImg = std::make_unique<BINDU::Sprite>();
	bndryImg->SetTexture(boundaryTexture);
	bndryImg->setPosition(0.f + static_cast<float>(m_boundaryImgOffset), (TILE_SIZE.y * 5) - static_cast<float>(m_boundaryImgOffset));
	bndryImg->setSize((TILE_SIZE.x * 20.f) - 2.f * static_cast<float>( m_boundaryImgOffset), TILE_SIZE.y * 40.f);
	bndryImg->setActive(true);
	m_boundaryImg = bndryImg.get();

	BINDU::Texture bndryShadowTex(RelativeResourcePath("Resource/images/border-shadow.png").c_str());

	std::unique_ptr<BINDU::Sprite> bndryShadow = std::make_unique<BINDU::Sprite>();
	bndryShadow->SetTexture(bndryShadowTex);
	bndryShadow->setPosition(bndryImg->getPosition() + 12.f);
	bndryShadow->setSize(bndryImg->getSize());
	bndryShadow->setActive(true);

	//for (int i = 0; i < 3; i++)
	//{
	//	std::unique_ptr<Ball> ball = std::make_unique<Ball>(BINDU::Vec2f{ 400.f,400.f }, BINDU::Bnd_Size_F{ 14.f,14.f }, BINDU::Vec2f{ 100.f,100.f });
	//	ball->Init();
	//	ball->onLoadResource();
	//	ball->setCollideAble(true);
	//	ball->setColliderType(BINDU::ColliderType::CIRCLE_COLLIDER);
	//	ball->setActive(true);
	//	m_currentBall = ball.get();
	//	m_balls.push_back(ball.get());
	//}
		

	std::unique_ptr<Paddle> paddle = std::make_unique<Paddle>();
	paddle->Init();
	paddle->onLoadResource();
	paddle->setPosition(400, 600);
	paddle->setSize(80, 25);
	paddle->setCollideAble(true);
	paddle->setColliderType(BINDU::ColliderType::RECT_COLLIDER);
	paddle->setCollider({ 0,0,60,16 });
	paddle->setActive(true);
	m_paddle = paddle.get();

	std::unique_ptr<BINDU::Layer> bgLayer = std::make_unique<BINDU::Layer>();
	bgLayer->setActive(true);
	bgLayer->AddObject(std::move(bgImg), "bgimg");
	bgLayer->setParallaxFactor({ 0.1f,0.1f });
	//bgLayer->setParallaxFactor({ 1.0f,1.0f });
	m_bgLayer1 = bgLayer.get();

	AddLayer(std::move(bgLayer), "bglayer");


	m_shadowTexture.LoadFromFile(RelativeResourcePath("Resource/images/brick-dropshadow.png").c_str());


	SetParticleEmitter();

	m_mapParser.setResourcePath(RelativeResourcePath("Resource/").c_str());


	//int lvlCounter = 1;
	//while (1)
	//{

	//	tinyxml2::XMLDocument doc;
	//	std::string lvlPath = std::string("Resource/levels/level" + std::to_string(lvlCounter) + ".xml");
	//	doc.LoadFile(RelativeResourcePath(lvlPath.c_str()).c_str());

	//	if (doc.Error())
	//		break;

	//	m_mapParser.Load(RelativeResourcePath(lvlPath.c_str()).c_str(), *this);
	//	lvlCounter++;
	//}


	std::unique_ptr<BINDU::Layer> shadowLayer = std::make_unique<BINDU::Layer>();

	shadowLayer->setActive(true);
	shadowLayer->setParallaxFactor({ 0.0f,0.0f });
	shadowLayer->setUpdating(false);
	shadowLayer->AddObject(std::move(bndryShadow), "bndryshadow");

	m_shadowLayer = shadowLayer.get();

	AddLayer(std::move(shadowLayer), "shadowlayer");

	m_mapParser.Load(RelativeResourcePath("Resource/levels/level1.xml").c_str(), *this);

	m_switchLevel = true;

	std::unique_ptr<BINDU::Layer> fgLayer = std::make_unique<BINDU::Layer>();
	fgLayer->setActive(true);
	fgLayer->AddObject(std::move(bndryImg), "bndryimg");
	fgLayer->AddObject(std::move(paddle), "paddle");

	for (int i = 0; i < m_maximumBalls; i++)
	{
		std::unique_ptr<Ball> ball = std::make_unique<Ball>(BINDU::Vec2f{ 400.f,400.f }, BINDU::Bnd_Size_F{ 14.f,14.f }, BINDU::Vec2f{ 100.f,100.f });
		ball->Init();
		ball->onLoadResource();
		ball->setOrigin(ball->getWidth() * 0.5f, ball->getHeight() * 0.5f);
		ball->setCollideAble(true);
		ball->setColliderType(BINDU::ColliderType::CIRCLE_COLLIDER);
		ball->setActive(false);
		m_balls.push_back(ball.get());
		fgLayer->AddObject(std::move(ball), std::string("ball" + std::to_string(i)).c_str());
	}
	m_currentBall = m_balls.back();
	m_currentBall->setActive(true);
	++m_activeBalls;

	fgLayer->setParallaxFactor({ 0.0f,0.0f });
	fgLayer->showDebug(false);
	m_fgLayer = fgLayer.get();


	AddLayer(std::move(fgLayer), "fglayer");


	m_font.LoadBitmapFont(RelativeResourcePath("Resource/images/unispace-bitmapfont.png").c_str());
	m_font.LoadWidthData(RelativeResourcePath("Resource/images/unispace-fontwidth.dat").c_str());
	m_font.setCharSize(20, 20);

	for(auto& m:m_layers)
	{
		if(m->getGuid() != "bglayer" && m->getGuid() != "bglayer2" && m->getGuid() != "fglayer" && m.get() != m_tileLayer && m->getGuid() != "shadowlayer")
		{
			m->setActive(false);
		}
		if (BINDU::TileLayer* tileLayer = dynamic_cast<BINDU::TileLayer*>(m.get()))
			++m_totalLevel;
		
	}

	SetUpHealth();

	BINDU::Texture transparentTex(RelativeResourcePath("Resource/images/transparent-sheet.png").c_str());
	std::unique_ptr<BINDU::Sprite>  transparentSprite = std::make_unique<BINDU::Sprite>();
	transparentSprite->setPosition(m_boundaryImg->getPosition().x + 3 * m_boundaryImgOffset, m_boundaryImg->getPosition().y + 3 * m_boundaryImgOffset);
	transparentSprite->setSize(m_boundaryImg->getPosition().x + m_boundaryImg->getWidth() - 7 * m_boundaryImgOffset, m_boundaryImg->getPosition().y + m_boundaryImg->getHeight() - 2 * m_boundaryImgOffset);
	transparentSprite->SetTexture(transparentTex);
	transparentSprite->setOpacity(155.f);
	transparentSprite->setActive(true);

	std::unique_ptr<BINDU::Layer> tLayer = std::make_unique<BINDU::Layer>();
	tLayer->setActive(true);
	tLayer->setParallaxFactor({ 0.f,0.f });
	m_transparentLayer = tLayer.get();

	tLayer->AddObject(std::move(transparentSprite), "transparentsprite");

	AddLayer(std::move(tLayer), "transparentlayer");

	SetUpPauseLayer();

	Scene::onLoadResource();

}


void MainScene::onReleaseResource()
{
	Scene::onReleaseResource();
}

void MainScene::Update(float dt)
{
	Scene::Update(dt);

//	m_camera.setBottomRight(m_camera.getBottomRight().x + (800.f - m_camera.getBottomRight().x) * 0.05f, m_camera.getBottomRight().x + (1200.f - m_camera.getBottomRight().y) * 0.05f);
	m_camera.setTopLeft(m_camera.getTopLeft().x + (0.f - m_camera.getTopLeft().x) * 0.2f, m_camera.getTopLeft().y + (0.f - m_camera.getTopLeft().y) * 0.2f);
	m_camera.setRotation(m_camera.getRotation() + (0.0f - m_camera.getRotation()) * 0.2f);
	m_camera.setZoom({ m_camera.getZoom().x + (1.0f - m_camera.getZoom().x) * 0.2f, 1.0f});
	m_camera.Update(dt, m_currentBall->getCenter());
	

	if(m_currentState != State::PAUSE)
	{
		UpdatePowerUps(dt);							// Updating the power-ups
		UpdateBullet(dt);							// Updating the bullets
		UpdateHealth();

		if (m_paddle->getPosition().x <= m_boundaryImg->getPosition().x + 3 * m_boundaryImgOffset)			// restricts paddle to go outside left border
		{
			m_paddle->setVelocity({ 0.f,0.f });
			m_paddle->setX(m_paddle->getX() + 2.f);
		}
		if (m_paddle->getPosition().x + m_paddle->getRectCollider().w >= m_boundaryImg->getPosition().x + m_boundaryImg->getWidth() - 3 * m_boundaryImgOffset)			// restricts paddle to go outside right border
		{
			m_paddle->setVelocity({ 0.f,0.f });
			m_paddle->setX(m_paddle->getX() - 2.f);
		}


		if (m_gameOver == true)
		{
			GameOver();
			m_retryButton->setActive(true);
		}

	}


	if(m_currentState == State::PLAY)
	{
		if (m_pauseLayer->isActive())
			m_pauseLayer->setActive(false);
		if (m_transparentLayer->isActive())
			m_transparentLayer->setActive(false);
		

		BINDU::Vec2f normal;										// collision normal

		float  overlap{};											// collision overlap



		for (const auto& ball : m_balls)						// Iterate through all the balls
		{
			if (ball->isActive())										// if it's active
			{
				if (ball->getCenter().x - ball->getSize().width * 0.5f <= m_boundaryImg->getPosition().x + 3 * m_boundaryImgOffset)		// ball hits left border
				{
					ball->setX(ball->getX() + 2.f);
					ball->setVelocity({ ball->getVelocity().x + 10.f,ball->getVelocity().y });
					ball->setVelocity({ -ball->getVelocity().x,ball->getVelocity().y });
					ball->setBounce(true);
					m_camera.setTopLeft(-400.f, 0.f);
				}
				else if (ball->getCenter().x + ball->getSize().width * 0.5f >= m_boundaryImg->getPosition().x + m_boundaryImg->getWidth() - 3 * m_boundaryImgOffset)		// ball hits right border
				{
					ball->setX(ball->getX() - 2.f);
					ball->setVelocity({ ball->getVelocity().x + 10.f,ball->getVelocity().y });
					ball->setVelocity({ -ball->getVelocity().x,ball->getVelocity().y });
					ball->setBounce(true);
					m_camera.setTopLeft(800.f, 0.f);
				}
				else if (ball->getCenter().y - ball->getSize().height * 0.5f <= m_boundaryImg->getPosition().y + 3 * m_boundaryImgOffset)		// ball hits top border
				{
					ball->setY(ball->getY() + 2.f);
					ball->setVelocity({ ball->getVelocity().x ,ball->getVelocity().y + 10.f });
					ball->setVelocity({ ball->getVelocity().x,-ball->getVelocity().y });
					ball->setBounce(true);
					m_camera.setTopLeft(0.f, -400.f);
				}
				else if (ball->getCenter().y + ball->getSize().height * 0.5f >= m_boundaryImg->getPosition().y + m_boundaryImg->getHeight() - 2 * m_boundaryImgOffset)		// ball hits bottom border
				{
					--m_activeBalls;

					/*if (m_activeBalls == 0)
					{
						m_ballDropped = true;
						m_currentBall = m;
						m_activeBalls = 1;
						break;
					}*/


					const auto found = std::find_if(m_balls.begin(), m_balls.end(), [&](const Ball* pBall) {return pBall != ball && pBall->isActive(); });

					if (found != m_balls.end())
					{
						ball->Reset();
						ball->setActive(false);
						m_currentBall = (*found);
					}
					else
					{
						m_ballDropped = true;
						if (GameManager::getSfx())
							GameManager::getSoundSystem()->Play("lifelost", false);
						ball->Reset();
						m_currentBall = ball;
						m_activeBalls = 1;
					}
					//		m_ball->setY(m_ball->getY() - 2.f);
					//		m_ball->setVelocity({ m_ball->getVelocity().x ,m_ball->getVelocity().y + 5.f });
					//		m_ball->setVelocity({ m_ball->getVelocity().x,-m_ball->getVelocity().y });
					
				}

				
				if (BINDU::CollisionHandler::CircleVsRect(ball->getCircleCollider(), ball->getVelocity(), m_paddle->getRectCollider(), dt, normal, overlap))		// check ball and paddle collision
				{
					if (m_paddle->getPaddleType() != PaddleType::MAGNETIC)			// if paddle type is not magnetic
					{
						ball->setPosition(ball->getPosition() - (normal * overlap));
						BINDU::Vec2f tempVelocity = ball->getVelocity();

						if (normal.y == 1.0f || normal.y == -1.0f)
							tempVelocity.y = -tempVelocity.y;
						else if (normal.x == 1.0f || normal.x == -1.0f)
							tempVelocity.x = -tempVelocity.x;

						if (ball->getCenter().x > m_paddle->getPosition().x + m_paddle->getSize().width / 2.f)
							tempVelocity.x += 20.f;
						if (ball->getCenter().x < m_paddle->getPosition().x + m_paddle->getSize().width / 2.f)
							tempVelocity.x -= 20.f;

						tempVelocity += m_paddle->getVelocity() * 0.50f;

						ball->setVelocity(tempVelocity);
					}
					else                                                             // if paddle type is magnetic
					{
						ball->setStickyBall(true);
						m_launchBall = false;
						ball->setDelta((m_paddle->getX() + m_paddle->getRectCollider().w) - ball->getX());
					}

					m_camera.setTopLeft(-400.f,  1000.f);
					ball->setBounce(true);
				}


				if (ball->isStickyBall())											// if the ball is sticky
				{
					ball->setPosition(((m_paddle->getX() + m_paddle->getRectCollider().w) - ball->getDelta()), ball->getY() - normal.y * overlap);
					ball->setVelocity({ 0.f, 0.f });
				}				

			}
		}

		if (m_ballDropped)													// if ball is dropped
		{
			m_currentState = State::TRANSITION;

			m_ballDropped = false;
			m_score = 0;
			m_powerUps.clear();
			m_paddle->setPaddleType(PaddleType::MID);
		//	UpdateHealth();
			DecreaseHealth();
		}

		int currCount = 0;
		for (auto& m : m_tileLayer->getTileMap())				// iterate through the TileMap
		{
			if (m)															// if m != nullptr
			{
				
				for (const auto& ball : m_balls)					// iterate through the balls				
				{
					if (ball->isActive() && m->isCollideAble)				// if ball is active; check collision with that ball and the current iterated tile of the TileMap.
					{
						if (BINDU::CollisionHandler::CircleVsRect(ball->getCircleCollider(), ball->getVelocity(), { m->position.x,m->position.y,m->size.width,m->size.height }, dt, normal, overlap))
						{
							

							BINDU::Vec2f tempVelocity = ball->getVelocity();


							if (!ball->isRageMode())						// if ball is not in rage mode.
							{
								ball->setPosition(ball->getPosition() - (normal * overlap));

								if (normal.y == 1.0f || normal.y == -1.0f)
								{
									tempVelocity.y += 10.f * normal.y;
									tempVelocity.y = -tempVelocity.y;
								}
								else if (normal.x == 1.0f || normal.x == -1.0f)
								{
									tempVelocity.x += 10.f * normal.x;
									tempVelocity.x = -tempVelocity.x;
								}
							}

							ball->setVelocity(tempVelocity);
							ball->setBounce(true);

							//TODO: Add power ups;

							if (m_powerUps.size() < 2 && BINDU::RandomNumber::Get(1, 5) == 4 && m->id != 8 && m->id != 9 && m->id != 10)
							{

								std::unique_ptr<BINDU::Sprite> pwr = std::make_unique<BINDU::Sprite>();

								pwr->setPosition(m->position);
								pwr->setSize(m->size);
								pwr->setActive(true);
								pwr->setCollider({ 0.f,0.f,m->size.width,m->size.height });
								pwr->SetTexture(m_powerUpTex);
								const int frameNum = BINDU::RandomNumber::Get(1, 8);
								m_powerUpAnimator.setCurrentFrame(frameNum);
								pwr->setRect(m_powerUpAnimator.getCurrentFrame());
								pwr->setGuid(std::to_string(frameNum).c_str());
								m_powerUps.push_back(std::move(pwr));
								//m_fgLayer->AddObject(std::move(pwr), std::to_string(frameNum).c_str());

							}

							m->collisionNormal = normal;
							m_camera.setTopLeft(normal.x * -400, normal.y * -400);
							
							DropBrick(m.get(), dt);					// drop the collided brick.

							break;										// no need to check further. already have a collision with the tile and ball.
						}
					}
				}

				if (m->id != 13 && m->id != 12 && m->id != 11 && m->id != 10)
					++currCount;

				UpdateBrick(m, dt);									// update the brick
			}


		}

		m_currentBricks = currCount;

		if (currCount == 0)												// if no more bricks left
		{
			//m_currentState = State::TRANSITION;

			SwitchLevel();												// switch level

			m_switchLevel = true;

			if (m_currentLevel >= m_layers.size())
				m_currentLevel = 0;

		}


	}
	else if (m_currentState == State::TRANSITION)
	{
		m_transparentLayer->setActive(true);

		TransitionPhase();
	}
	else if(m_currentState == State::READY_PHASE)
	{
		m_transparentLayer->setActive(true);

		UpdateReadyPhase();
	}
	else if(m_currentState == State::PAUSE)
	{
		m_transparentLayer->setActive(true);

		if (GameManager::getMusic())
			GameManager::getSoundSystem()->FindSound("gamemusic")->setVolume(0.0f);


		if (!m_pauseLayer->isActive())
			m_pauseLayer->setActive(true);

		UpdatePauseLayer();
	}


	m_font.PrintText(0, 0, "REAL: " + std::to_string(g_engine->getRealFrameRate()));
	m_font.PrintText(0, 24, "CORE: " + std::to_string(g_engine->getCoreFrameRate()));


	m_font.PrintText(625, 260, "HIGH SCORE");
	m_font.PrintText(645, 290, std::to_string(m_highScore), { 255,255,0,255 });
	m_font.PrintText(625, 340, "SCORE");
	m_font.PrintText(645, 370, std::to_string(m_score), { 255,255,0,255 });
	m_font.PrintText(625, 420, "BRICKS");
	m_font.PrintText(645, 450, std::to_string(m_currentBricks), { 255,255,0,255 });

	m_font.PrintText(625, 500, std::string("CURRENT BALL: " + std::to_string(m_activeBalls)).c_str(),{255,255,255,255},0.9f);
}

void MainScene::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{

	Scene::Draw(graphics, m_camera.getCameraMatrix());


	for (const auto& m : m_powerUps)
	{
		if(m)
			m->Draw(graphics, cameraMatrix);
	}


	for(const auto& m: m_healths)
	{
		if (m)
			m->Draw(graphics, cameraMatrix);
	}

	// Drawing the grid
	//graphics->getRenderTarget()->SetTransform(D2D1::IdentityMatrix());
	//for (int y = static_cast<int>(m_boundaryImg->getPosition().y + (3 * m_boundaryImgOffset)); y <= m_boundaryImg->getSize().height ; y += TILE_SIZE.y)
	//{
	//	for (int x = static_cast<int>(m_boundaryImg->getPosition().x + (3 * m_boundaryImgOffset)); x <= m_boundaryImg->getSize().width - (3 * m_boundaryImgOffset); x += TILE_SIZE.x)
	//	{
	//			
	//			graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::DarkSlateGray));
	//			graphics->getRenderTarget()->DrawRectangle({ static_cast<float>(x),static_cast<float>(y),static_cast<float>(x) + static_cast<float>(TILE_SIZE.x),static_cast<float>(y) + static_cast<float>(TILE_SIZE.y) }, graphics->getSolidColorBrush());
	//	}

	//}


	m_font.Draw(graphics, D2D1::IdentityMatrix());

	

}

void MainScene::ProcessInput()
{

	if (m_currentState != State::PAUSE && !m_gameOver && BINDU::Input::isKeyPressed(BND_ESCAPE))
	{
		m_retryButton->setActive(false);

		m_transparentLayer->setActive(true);
		m_pauseLayer->setActive(true);

		for (const auto& m : m_layers)
		{
			if (m->getGuid() != "pauselayer")
				m->setUpdating(false);
		}
		m_prevState = m_currentState;
		m_currentState = State::PAUSE;
	}

	if(BINDU::Input::isKeyPressed(BND_Y))
	{
		SwitchLevel();

		if (m_currentLevel >= m_layers.size())
			m_currentLevel = 0;

	}

	if(m_currentState == State::READY_PHASE && BINDU::Input::isKeyPressed(BND_SPACE))
	{
		m_gameOver = false;
		m_readyPhase = false;
		m_readyPhaseTimer = 180;

		m_transparentLayer->setActive(false);

		m_currentState = State::PLAY;

	}

	if (BINDU::Input::isKeyPressed(BND_U))
		m_transparentLayer->setActive(true);


	m_mouseCell = { (BINDU::Input::getMousePosition().x),(BINDU::Input::getMousePosition().y) };

	for (auto& ball : m_balls)
	{
		if (ball->isActive())
		{
			if (ball->isStickyBall() && BINDU::Input::isKeyPressed(BND_SPACE))
			{
				//m_launchBall = true;
				LaunchBall(ball);
			}

			if (BINDU::Input::isMouseButtonHold(BND_BTN_LEFT))
			{
				ball->setPosition(m_mouseCell);
			}
		}
	}

	if (BINDU::Input::isKeyPressed(BND_R))
		ResetGame();


	if(m_currentState == State::PAUSE)
	{
		if (BINDU::Input::isKeyPressed(BND_ESCAPE))
			m_switchToPlay = true;

		if (BINDU::Input::isKeyPressed(BND_S) || BINDU::Input::isKeyPressed(BND_DOWN))
		{
			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("navsound", false, 1.0f);

			m_selector->setY(m_selector->getY() + 40);
		}
		else if (BINDU::Input::isKeyPressed(BND_W) || BINDU::Input::isKeyPressed(BND_UP))
		{
			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("navsound", false, 1.0f);

			m_selector->setY(m_selector->getY() - 40);
		}
			

		if (BINDU::Input::isKeyPressed(BND_ENTER))
		{
			if (!m_gameOver)
			{
				if (m_selector->getY() == m_resumeButton->getY())
				{
					m_buttonAnimator.setCurrentRow(1);
					m_switchToPlay = true;
					
				}
			}
			else
			{
				if (m_selector->getY() == m_retryButton->getY())
				{
					m_buttonAnimator.setCurrentRow(4);
					m_switchToPlay = true;
				}
			}

			if (m_selector->getY() == m_menuButton->getY())
			{
				m_buttonAnimator.setCurrentRow(3);
				m_switchToMenu = true;
			}
			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("clicksound", false, 1.0f);
		}
		else if(BINDU::Input::isMouseButtonPressed(BND_BTN_LEFT))
		{
			if (!m_gameOver)
			{
				if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()),m_resumeButton->getRectCollider()))
				{
					m_buttonAnimator.setCurrentRow(1);
					m_switchToPlay = true;

				}
			}
			else
			{
				if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_retryButton->getRectCollider()))
				{
					m_buttonAnimator.setCurrentRow(4);
					m_switchToPlay = true;
				}
			}

			if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_menuButton->getRectCollider()))
			{
				m_buttonAnimator.setCurrentRow(3);
				m_switchToMenu = true;
			}
			else if(BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_musicButton->getRectCollider()))
			{
				if(GameManager::getMusic())
				{
					m_musicButton->setRect({ 96.f,5.f * 32.f,96.f,32.f });
					GameManager::setMusic(false);
				}
				else
				{
					m_musicButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });
					GameManager::setMusic(true);
				}
			}
			else if(BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_sfxButton->getRectCollider()))
			{
				if (GameManager::getSfx())
				{
					m_sfxButton->setRect({ 96.f,5.f * 32.f,96.f,32.f });
					GameManager::setSfx(false);
				}
				else if (!GameManager::getSfx())
				{
					m_sfxButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });
					GameManager::setSfx(true);
				}
			}

			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("clicksound", false, 1.0f);
		}
	}

	Scene::ProcessInput();

}

void MainScene::ResetGame()
{
	for (auto& m : m_layers)
	{
		if (BINDU::TileLayer* tileLayer = dynamic_cast<BINDU::TileLayer*>(m.get()))
			tileLayer->onResetTilemap();
	}

	m_tileLayer = nullptr;

	m_currentBall->Reset();
	m_paddle->getBullets().clear();
	m_powerUps.clear();

	m_healths.clear();
	SetUpHealth();

	m_currentLevel = 0;
	m_gameOver = false;
	
	m_currentState = State::TRANSITION;
}

void MainScene::GameComplete()
{

}

void MainScene::SwitchLevel()
{
	if (GameManager::getSfx())
		GameManager::getSoundSystem()->Play("levelcomplete", false);

	for (auto& m : m_layers)
	{
		if (BINDU::TileLayer* tileLayer = dynamic_cast<BINDU::TileLayer*>(m.get()))
		{
			if (m_currentLevel == m_totalLevel)
				m_currentLevel = 0;

			std::string lvlName = tileLayer->getGuid();
			const size_t pos = lvlName.find_last_of('l');
			lvlName = lvlName.substr(pos + 1, lvlName.size() - pos);
			const int lvl = static_cast<int>(std::strtol(lvlName.c_str(), nullptr, 0));
			if (lvl > m_currentLevel)
			{
				m_currentLevel = lvl;
				m_tileLayer = tileLayer;
				break;
			}
		}
	}

	m_powerUps.clear();
	m_paddle->getBullets().clear();
	m_shadowLayer->RemoveExcept("bndryshadow");
	

	if (m_tileLayer)
	{
		for (auto& m : m_layers)
		{
			if (m.get() != m_tileLayer && m->getGuid() != "fglayer" && m->getGuid() != "bglayer" && m->getGuid() != "bglayer2" && m->getGuid() != "shadowlayer")
			{
				m->setActive(false);
			}
		}

		m_tileLayer->setActive(true);
		m_tileLayer->onResetTilemap();
		m_tileLayer->setParallaxFactor({ 0.0f,0.0f });

		m_switchLevel = false;
		m_readyPhase = true;
		m_transitionPhase = true;

		for (auto& m : m_tileLayer->getTileMap())
		{
			if (m)
			{
				m->sprite.setScale(0.1f, 0.1f);

				std::unique_ptr<BINDU::Sprite> shadowSprite = std::make_unique<BINDU::Sprite>();
				shadowSprite->setActive(true);
				shadowSprite->setPosition(m->position.x + 10.f,m->position.y + 10.f);
				shadowSprite->setSize(m->size);
				shadowSprite->SetTexture(m_shadowTexture);
				shadowSprite->setCollideAble(false);

				
				m_shadowLayer->setParallaxFactor(m_tileLayer->getParallaxFactor());
				m_shadowLayer->AddObject(std::move(shadowSprite), m->sprite.getGuid().c_str());

			}
		}

	}

	m_currentState = State::TRANSITION;
}

void MainScene::GameOver()
{
	m_currentState = State::PAUSE;


}

void MainScene::TransitionPhase()
{
	if(!m_tileLayer)
	{
		SwitchLevel();
	}

	for (const auto& m : m_layers)
	{
		if (m->getGuid() != "pauselayer" && !m->isUpdating())
			m->setUpdating(true);
	}

	for (const auto& ball : m_balls)
	{
		ball->setPosition({ m_paddle->getX() + m_paddle->getRectCollider().w * 0.5f - ball->getSize().width * 0.5f,m_paddle->getY() - ball->getSize().height });
	}

	for(const auto& m : m_tileLayer->getTileMap())
	{
		if(m && m->sprite.getScale().x >= 1.0f)
		{
			m->sprite.setScale(0.1f, 0.1f);
		}
	}
	for (auto& m : m_tileLayer->getTileMap())
	{
		if (m)
		{
			//m->sprite.setScale(m->sprite.getScale() * 1.1f);
			m->sprite.setScale(1.0f + (m->sprite.getScale().x - 1.0f) * 0.95f, 1.0f + (m->sprite.getScale().y - 1.0f) * 0.94f);
			if (m->sprite.getScale().x < 0.99f)
			{
			/*	m_transitionPhase = true;
				m->sprite.setScale(1.f, 1.f);
				m_transitionPhase = false;*/

				m_currentState = State::TRANSITION;
			}
			else
			{
				m->sprite.setScale(1.f, 1.f);
				m_currentState = State::READY_PHASE;
			}
		}
	}
}

void MainScene::UpdateReadyPhase()
{

	if(m_readyPhaseTimer > 0)
	{
		--m_readyPhaseTimer;

		if (m_healths.size() > 0)
			m_font.PrintText(320, 350, "LEVEL: " + std::to_string(m_currentLevel), { 255,0,0,255 }, 2.0f);
		
	}
	else
	{
		m_font.PrintText(320, 350, "Ready?", { 255,0,0,255 }, 2.0f);
	}

	for(const auto& ball:m_balls)
	{
		ball->setPosition({ m_paddle->getX() + m_paddle->getRectCollider().w * 0.5f - ball->getSize().width * 0.5f,m_paddle->getY() - ball->getSize().height });
	}

	if (m_paddle->getCenter().x < m_boundaryImg->getX() + m_boundaryImg->getWidth() * 0.5f)
		m_currentBall->setVelocity({ 120.f,-100.f });
	else if (m_paddle->getCenter().x > m_boundaryImg->getX() + m_boundaryImg->getWidth() * 0.5f)
		m_currentBall->setVelocity({ -120.f,-100.f });
	else
		m_currentBall->setVelocity({ 0.f,-100.f });
	
}

void MainScene::SetParticleEmitter()
{

	BINDU::ParticleProps props;

	props.size = { 4.f,4.f };
	props.velocity = { 5.f,5.f };
	props.startScale = 1.0f;
	props.endScale = 2.0f;
	props.startColor = { 144,80,55,200 };
	props.endColor = { 144,80,55,20 };
//	props.colorRandomnessRange = BINDU::iRange(0, 255);
//	props.colorOpacityRange = BINDU::iRange(0, 255);
	props.minTimetoChangeColor = 7.5f;
	props.fadeOut = true;
	props.rotation = 0;
	props.rotationRate = 20;
	props.lifeTime = 10.0f;

	std::unique_ptr<BINDU::ParticleEmitter> emitter = std::make_unique<BINDU::ParticleEmitter>();

	BINDU::Texture tex(RelativeResourcePath("Resource/images/particle16.png").c_str());

	emitter->Init(props);
	emitter->SetTexture(tex);
	emitter->setSize(800, 720);
	emitter->onLoop(true);
	emitter->setDirection(100);
	emitter->setPosition(0.f,0.f);
	emitter->setMax(200);
	emitter->setSpread(90);
	emitter->setEmissionInterval(50);
	emitter->setEmissionRate(1);
	emitter->setActive(true);
	m_particleEmitter = emitter.get();

	std::unique_ptr<BINDU::Layer> bglayer2 = std::make_unique<BINDU::Layer>();
	bglayer2->setActive(true);
	bglayer2->setParallaxFactor({0.2f,0.2f});
	//bglayer2->setParallaxFactor({ 0.0f,0.0f });
	m_bgLayer2 = bglayer2.get();

	bglayer2->AddObject(std::move(emitter), "emitter");
	bglayer2->setActive(true);

	AddLayer(std::move(bglayer2), "bglayer2");

}

void MainScene::UpdatePowerUps(const float dt)
{
	for (auto itr = m_powerUps.begin(); itr != m_powerUps.end();)
	{
		if((*itr))
		{
						
				(*itr)->setY((*itr)->getY() + 50 * dt);
				BINDU::Bnd_Rect_F powerUpRect = { (*itr)->getX(), (*itr)->getY(), (*itr)->getSize().width, (*itr)->getSize().height };

				if (BINDU::CollisionHandler::RectVsRect(m_paddle->getRectCollider(), powerUpRect))
				{
					std::string id = (*itr)->getGuid();
					if (id == "1")
						m_paddle->setPaddleType(PaddleType::SMALL);
					else if (id == "2")
						m_paddle->setPaddleType(PaddleType::LARGE);
					else if (id == "3")
						m_paddle->setPaddleType(PaddleType::MAGNETIC);
					else if (id == "4")
						m_paddle->setPaddleType(PaddleType::LASER);
					else if (id == "5")
					{
						for (const auto& ball : m_balls)
						{
							if (ball != m_currentBall && !ball->isActive() && m_activeBalls < m_maximumBalls)
							{
								++m_activeBalls;
								ball->setActive(true);
								ball->setPosition({ m_paddle->getX() + m_paddle->getRectCollider().w * 0.5f - ball->getSize().width * 0.5f,m_paddle->getY() - ball->getSize().height });
								ball->setSize(m_currentBall->getSize());
								ball->setOrigin(ball->getWidth() * 0.5f, ball->getHeight() * 0.5f);

								const float dx = static_cast<float>(BINDU::RandomNumber::Get(100, 150));
								const float dy = static_cast<float>(BINDU::RandomNumber::Get(100, 150));
								ball->setVelocity({ dx,-dy });

								if (m_activeBalls > 3)
									m_activeBalls = 3;

							}
						}

					}
					else if (id == "6")
					{
						for (const auto& ball : m_balls)
						{
							if (ball->isActive())
							{
								ball->setRageMode(true);
							}
						}
					}
					else if (id == "7")
						IncreaseHealth();
					else if (id == "8")
					{
						for (const auto& ball : m_balls)
						{
							if (ball->isActive())
							{
								BINDU::Vec2f tempVel = ball->getVelocity() * 0.7f;

								if (tempVel.x < 100.f && tempVel.x > 0)
									tempVel.x = 100.f;
								if (tempVel.x > -100.f && tempVel.x <= 0)
									tempVel.x = -100.f;
								if (tempVel.y < 100.f && tempVel.y > 0)
									tempVel.y = 100.f;
								if (tempVel.y > -100.f && tempVel.y <= 0)
									tempVel.y = -100.f;

								ball->setVelocity(tempVel);
							}
						}
					}


					itr = m_powerUps.erase(itr);

					if (GameManager::getSfx())
						GameManager::getSoundSystem()->Play("powerup", false);
				}
				else if ((*itr)->getY() >= m_boundaryImg->getPosition().y + m_boundaryImg->getHeight() - 2 * m_boundaryImgOffset)
					itr = m_powerUps.erase(itr);
				else
					++itr;

		}
		else
		{
			itr = m_powerUps.erase(itr);
			++itr;
		}
	}


}

void MainScene::UpdateBullet(const float dt)
{

	for(auto itr = m_paddle->getBullets().begin(); itr!=m_paddle->getBullets().end();)
	{
		if (*itr)
		{
			(*itr)->setY((*itr)->getY() - 350.f * dt);
			(*itr)->scaleY((*itr)->getScale().y + (4.0f - (*itr)->getScale().y) * 0.1f);

			BINDU::Bnd_Rect_F bulletRect = { (*itr)->getX(), (*itr)->getY(), (*itr)->getSize().width, (*itr)->getSize().height };
			bool collides = false;

			for (auto& m : m_tileLayer->getTileMap())
			{
				if (m && m->isCollideAble)
				{
					if (BINDU::CollisionHandler::RectVsRect(bulletRect, { m->position.x,m->position.y,m->size.width,m->size.height }))
					{
						//m.reset();
						collides = true;

						DropBrick(m.get(), dt);

						break;
					}

				}
			}

			if (collides)
				itr = m_paddle->getBullets().erase(itr);
			else if((*itr)->getY() <= m_boundaryImg->getY() + 3 * m_boundaryImgOffset)
				itr = m_paddle->getBullets().erase(itr);
			else
			++itr;
		}
		else
		{
			itr = m_paddle->getBullets().erase(itr);
			++itr;
		}
	}

}

void MainScene::UpdateBrick(std::unique_ptr<BINDU::Tile>& tile, const float dt)
{

	if (tile->sprite.getOpacity() < 250.f)
	{
		tile->isCollideAble = false;
		tile->sprite.setScale(1.0f + (tile->sprite.getScale().x - 1.0f) * 0.90f, 1.0f + (tile->sprite.getScale().y - 1.0f) * 0.90f);

		if (tile->collisionNormal.y > 0)
			tile->sprite.setRotation(++tile->collisionNormal.y);
		else if (tile->collisionNormal.y < 0)
			tile->sprite.setRotation(--tile->collisionNormal.y);
		else if (tile->collisionNormal.x > 0)
			tile->sprite.setRotation(++tile->collisionNormal.x);
		else
			tile->sprite.setRotation(--tile->collisionNormal.x);

		tile->position.y = tile->position.y + 100.f * dt;
		tile->sprite.setOpacity(4.f + (tile->sprite.getOpacity() - 4.f) * 0.92f);

		if (tile->sprite.getOpacity() < 5.f)
		{
			tile.reset();
			m_score++;
			if (m_score > m_highScore)
				m_highScore = m_score;
		}
	}
	else
	{
		tile->sprite.setScale(1.0f + (tile->sprite.getScale().x - 1.0f) * 0.90f, 1.0f + (tile->sprite.getScale().y - 1.0f) * 0.90f);
	}
}

void MainScene::DropBrick(BINDU::Tile* tile, const float dt)
{
	BINDU::Bnd_Rect_F srcRect;

	m_camera.setRotation(20.f * dt);
	if (m_camera.getRotation() > 20.f * dt || m_camera.getRotation() < 20.f * dt)
		m_camera.setRotation(0.f);

	m_camera.setZoom({ 1.01f, 1.0f });

	tile->sprite.setScale(1.5f + (tile->sprite.getScale().x - 1.5f) * dt, 1.5f + (tile->sprite.getScale().y - 1.5f) * dt);
	tile->sprite.setOrigin(tile->size.width * 0.5f, tile->size.height * 0.5f);

	if (tile->collisionNormal.y > 0)
		tile->sprite.setRotation(tile->collisionNormal.y += 5.f);
	else if (tile->collisionNormal.y < 0)
		tile->sprite.setRotation(tile->collisionNormal.y -= 5.f);
	else if (tile->collisionNormal.x > 0)
		tile->sprite.setRotation(tile->collisionNormal.x += 5.f);
	else
		tile->sprite.setRotation(tile->collisionNormal.x -= 5.f);

	if (tile->id == 10)
	{
		tile->id = 11;
		tile->animator.setCurrentFrame(12);
		srcRect = tile->animator.getCurrentFrame();
		tile->sprite.setRect(srcRect);
	}
	else if (tile->id == 11)
	{
		tile->id = 12;
		tile->animator.setCurrentFrame(13);
		srcRect = tile->animator.getCurrentFrame();
		tile->sprite.setRect(srcRect);
	}
	else if(tile->id == 13)
	{
		//TODO: Add behaviour to tile 13
		if (tile->sprite.getRotation() > 10.f)
			tile->sprite.setRotation(10.f);
		if (tile->sprite.getRotation() < -10.f)
			tile->sprite.setRotation(-10.f);
	}
	else
	{
		tile->sprite.setOpacity(tile->sprite.getOpacity() - 50.f);
		m_shadowLayer->RemoveObject(tile->sprite.getGuid().c_str());
	}

	if (GameManager::getSfx())
		GameManager::getSoundSystem()->Play("brickfall", false);

}

void MainScene::SetUpHealth()
{
	if (m_healths.size() < 1)
	{
		const float posX = 650.f;
		const float posY = 200.f;
		const BINDU::Bnd_Size_F size = { 16.f,16.f };

		for (int i = 0; i < m_totalHealth; i++)
		{
			std::unique_ptr<BINDU::Sprite> health = std::make_unique<BINDU::Sprite>();
			health->SetTexture(m_healthTex);
			health->setPosition(posX + size.width * 1.5f * i, posY);
			health->setScale(0.1f, 0.1f);
			health->setOpacity(0.0f);
			health->setOrigin(health->getSize().width * 0.5f, health->getSize().height * 0.5f);
			health->setSize(size);

			m_healths.push_back(std::move(health));

		}
	}
}

void MainScene::DecreaseHealth() const
{
	m_healths.back()->setScale(3.0f, 3.0f);
}

void MainScene::IncreaseHealth()
{
	if (m_healths.size() < m_totalHealth)
	{
		std::unique_ptr<BINDU::Sprite> health = std::make_unique<BINDU::Sprite>();
		health->SetTexture(m_healthTex);
		health->setPosition(m_healths.back()->getX() + (m_healths.back()->getWidth() * m_healths.back()->getScale().x) + 5.f, m_healths.back()->getY() + m_healths.back()->getSize().height * 0.25f);
		health->setSize(m_healths.back()->getSize());
		health->setScale(0.1f, 0.1f);
		health->setOpacity(0.0f);
		health->setOrigin(health->getSize().width * 0.5f, health->getSize().height * 0.5f);
		m_healths.push_back(std::move(health));
	}
}

void MainScene::UpdateHealth()
{

	if(m_healths.size() < 1)
	{
		m_gameOver = true;
	}
	else
	{
		if (m_healths.back()->getScale().x > 1.5f)
		{
			m_healths.back()->scaleX(m_healths.back()->getScale().x + (1.55f - m_healths.back()->getScale().x) * 0.1f);
			m_healths.back()->scaleY(m_healths.back()->getScale().y + (1.55f - m_healths.back()->getScale().y) * 0.1f);
			m_healths.back()->setOpacity(m_healths.back()->getOpacity() + (0.0f - m_healths.back()->getOpacity()) * 0.1f);
		}
	
		if (m_healths.back()->getScale().x > 1.5f && m_healths.back()->getScale().x < 1.6f)
		{
			m_healths.back()->setScale(1.5f, 1.5f);
			m_healths.pop_back();
		}

		for (const auto& health : m_healths)
		{
			if (health->getScale().x < 1.5f)
			{
				health->scaleX(health->getScale().x + (1.45f - health->getScale().x) * 0.1f);
				health->scaleY(health->getScale().y + (1.45f - health->getScale().y) * 0.1f);
				health->setOpacity(health->getOpacity() + (255.f - health->getOpacity()) * 0.1f);
			}
			if (health->getScale().x < 1.5f && health->getScale().x > 1.4f)
			{
				health->setScale(1.5f, 1.5f);
			}
		}
			
	}
}

void MainScene::SetUpPauseLayer()
{
	BINDU::Texture btnTex(RelativeResourcePath("Resource/images/buttons.png").c_str());
	BINDU::Texture slctTex(RelativeResourcePath("Resource/images/selector2.png").c_str());

	std::unique_ptr<BINDU::Sprite> resumeButton = std::make_unique<BINDU::Sprite>();
	resumeButton->SetTexture(btnTex);
	resumeButton->setPosition(200, 100);
	resumeButton->setSize(96, 32);
	resumeButton->setRect({ 0.f,0.f * 32.f,96,32 });
	resumeButton->setCollider({ 0.f,0.f,resumeButton->getSize().width,resumeButton->getSize().height });
	resumeButton->setActive(true);
	m_resumeButton = resumeButton.get();

	std::unique_ptr<BINDU::Sprite> retryButton = std::make_unique<BINDU::Sprite>();
	retryButton->SetTexture(btnTex);
	retryButton->setPosition(200, 100);
	retryButton->setSize(96, 32);
	retryButton->setRect({ 0.f,3.f * 32.f,96,32 });
	retryButton->setCollider({ 0.f,0.f,retryButton->getSize().width,retryButton->getSize().height });
	retryButton->setActive(false);
	m_retryButton = retryButton.get();

	std::unique_ptr<BINDU::Sprite> menuButton = std::make_unique<BINDU::Sprite>();
	menuButton->SetTexture(btnTex);
	menuButton->setPosition(200, 140);
	menuButton->setSize(96, 32);
	menuButton->setRect({ 0.f,2.f * 32.f,96,32 });
	menuButton->setCollider({ 0.f,0.f,resumeButton->getSize().width,resumeButton->getSize().height });
	menuButton->setActive(true);
	m_menuButton = menuButton.get();

	m_buttonAnimator.setTotalColumn(4);
	m_buttonAnimator.setTotalFrame(4);
	m_buttonAnimator.setFrameSize(96.f, 32.f);
	m_buttonAnimator.setCurrentRow(1);
	m_buttonAnimator.setFrameTime(200);

	std::unique_ptr<BINDU::Sprite> scltrSprite = std::make_unique<BINDU::Sprite>();
	scltrSprite->SetTexture(slctTex);
	scltrSprite->setPosition(200, 100);
	scltrSprite->setSize(96, 32);
	scltrSprite->setActive(true);
	m_selector = scltrSprite.get();

	m_selectorAnimator.setTotalColumn(4);
	m_selectorAnimator.setTotalFrame(4);
	m_selectorAnimator.setFrameSize(96.f, 32.f);
	m_selectorAnimator.setCurrentRow(1);
	m_selectorAnimator.setFrameTime(200);

	std::unique_ptr<BINDU::Layer> pauseLayer = std::make_unique<BINDU::Layer>();
	pauseLayer->setActive(false);
	pauseLayer->setParallaxFactor({ 0.0f,0.0f });

	pauseLayer->AddObject(std::move(resumeButton), "resumebutton");
	pauseLayer->AddObject(std::move(retryButton), "retrybutton");
	pauseLayer->AddObject(std::move(menuButton), "menubutton");
	pauseLayer->AddObject(std::move(scltrSprite), "selector");

	m_pauseLayer = pauseLayer.get();

	pauseLayer->setOffsetX(150);
	pauseLayer->setOffsetY(100);


	std::unique_ptr<BINDU::Sprite> musicButton = std::make_unique<BINDU::Sprite>();
	musicButton->setPosition(150.f, 180.f);
	musicButton->setSize(96.f, 32.f);
	musicButton->SetTexture(btnTex);
	musicButton->setCollider({ 32.f,4.f,32.f,28.f });
	musicButton->setActive(true);
	musicButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });

	m_musicButton = musicButton.get();

	std::unique_ptr<BINDU::Sprite> sfxButton = std::make_unique<BINDU::Sprite>();
	sfxButton->setPosition(250.f, 180.f);
	sfxButton->setSize(96.f, 32.f);
	sfxButton->SetTexture(btnTex);
	sfxButton->setCollider({ 32.f,4.f,32.f,28.f });
	sfxButton->setActive(true);
	sfxButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });

	m_sfxButton = sfxButton.get();

	pauseLayer->AddObject(std::move(musicButton), "musicbutton");
	pauseLayer->AddObject(std::move(sfxButton), "sfxbutton");

	AddLayer(std::move(pauseLayer), "pauselayer");

}

void MainScene::UpdatePauseLayer()
{
	BINDU::Bnd_Rect_F srcRect;
	m_selectorAnimator.Animate(srcRect);
	m_selector->setRect(srcRect);

	if (!m_gameOver)
	{
		m_retryButton->setActive(false);
		m_resumeButton->setActive(true);

		if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_resumeButton->getRectCollider()))
			m_selector->setY(m_resumeButton->getY());
		else if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_menuButton->getRectCollider()))
			m_selector->setY(m_menuButton->getY());

		if (m_selector->getY() > m_menuButton->getY())
			m_selector->setY(m_resumeButton->getY());
		else if (m_selector->getY() < m_resumeButton->getY())
			m_selector->setY(m_menuButton->getY());
	}
	else
	{
		m_retryButton->setActive(true);
		m_resumeButton->setActive(false);

		m_font.PrintText(270, 140, "GAME OVER", { 255.f,0.0f,0.0f,255.f },3.f);

		if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_retryButton->getRectCollider()))
			m_selector->setY(m_retryButton->getY());
		else if (BINDU::CollisionHandler::PointVsRect(m_mouseCell - BINDU::Vec2f(m_pauseLayer->getOffsetX(), m_pauseLayer->getOffsetY()), m_menuButton->getRectCollider()))
			m_selector->setY(m_menuButton->getY());

		if (m_selector->getY() > m_menuButton->getY())
			m_selector->setY(m_retryButton->getY());
		else if (m_selector->getY() < m_resumeButton->getY())
			m_selector->setY(m_menuButton->getY());
	}

	if(m_switchToPlay)
	{
		if(!m_gameOver)
		{
			m_resumeButton->setRect(m_buttonAnimator.Animate(2, 4));
			if(m_buttonAnimator.getCurrentFrameNum() == 4)
			{
					m_transparentLayer->setActive(false);
					m_pauseLayer->setActive(false);

					for (const auto& m : m_layers)
					{
						if (m->getGuid() != "pauselayer")
							m->setUpdating(true);
					}

					m_currentState = m_prevState;
					m_switchToPlay = false;
			}

		}else
		{
			m_retryButton->setRect(m_buttonAnimator.Animate(2, 4));
			if(m_buttonAnimator.getCurrentFrameNum() == 4)
			{
				m_transparentLayer->setActive(false);
				m_pauseLayer->setActive(false);

				m_gameOver = false;
				SetUpHealth();

				for (const auto& m : m_layers)
				{
					if (m->getGuid() != "pauselayer")
						m->setUpdating(true);
				}

				m_tileLayer->onResetTilemap();

				ResetShadows();

				m_currentState = State::TRANSITION;

				m_switchToPlay = false;
			}
		}

		GameManager::getSoundSystem()->FindSound("gamemusic")->setVolume(0.3f);
	}
	else if(m_switchToMenu)
	{
		m_menuButton->setRect(m_buttonAnimator.Animate(2, 4));
		if(m_buttonAnimator.getCurrentFrameNum() == 4)
		{
			m_transparentLayer->setActive(false);
			m_pauseLayer->setActive(false);

			GameManager::SetState(GameState::MENU);
			GameManager::getSoundSystem()->StopAll();
			ResetGame();
			m_switchToMenu = false;

		}
	
	}

	m_font.PrintText(260, 290, "MUSIC",{245.f,53.f,170.f,255.f});
	m_font.PrintText(390, 290, "SFX", { 245.f,53.f,170.f,255.f });

	if(GameManager::getMusic())
	{
		m_musicButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });
	}
	else
	{
		m_musicButton->setRect({ 96.f,5.f * 32.f,96.f,32.f });
	}

	if (GameManager::getSfx())
	{
		m_sfxButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });
	}
	else 
	{
		m_sfxButton->setRect({ 96.f,5.f * 32.f,96.f,32.f });
	}

}

void MainScene::ResetShadows() const
{
	m_shadowLayer->RemoveExcept("bndryshadow");

	for(const auto& m:m_tileLayer->getTileMap())
	{
		if (m)
		{
			std::unique_ptr<BINDU::Sprite> shadowSprite = std::make_unique<BINDU::Sprite>();
			shadowSprite->setActive(true);
			shadowSprite->setPosition(m->position.x + 10.f, m->position.y + 10.f);
			shadowSprite->setSize(m->size);
			shadowSprite->SetTexture(m_shadowTexture);
			shadowSprite->setCollideAble(false);


			m_shadowLayer->setParallaxFactor(m_tileLayer->getParallaxFactor());
			m_shadowLayer->AddObject(std::move(shadowSprite), m->sprite.getGuid().c_str());
		}
	}
}


void MainScene::LaunchBall(Ball* ball)
{
		ball->setStickyBall(false);

	if (ball->getCenter().x > m_paddle->getPosition().x + m_paddle->getSize().width * 0.5f)
		ball->setVelocity({ 80.f,-150.f });
	if (ball->getCenter().x < m_paddle->getPosition().x + m_paddle->getSize().width * 0.5f)
		ball->setVelocity({ -80.f,-150.f });
}
