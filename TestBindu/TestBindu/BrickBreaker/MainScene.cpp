#include "MainScene.h"

#include <algorithm>

#include "CollisionHandler.h"
#include "GameManager.h"
#include "Input.h"
#include "Random.h"


void MainScene::Init()
{
	m_circle.center = { 200,400 };
	circleVel = { 200,200 };
	m_circle.radius = 5;

	m_font.Init();

	m_camera.setViewSize(800, 720);
	m_camera.setBottomRight(800, 1280);
}

void MainScene::onLoadResource(const BINDU::Bnd_Size& windSize)
{
	BINDU::Texture bgTexture(RelativeResourcePath("Resource/images/background.png").c_str());
	BINDU::Texture boundaryTexture(RelativeResourcePath("Resource/images/borders.png").c_str());

	std::unique_ptr<BINDU::Sprite> bgImg = std::make_unique<BINDU::Sprite>();
	bgImg->SetTexture(bgTexture);
	bgImg->setPosition(0.f, 0.f);
	bgImg->setSize(static_cast<float>(windSize.width), static_cast<float>(windSize.height));
	bgImg->setActive(true);
	m_backgroundImg = bgImg.get();

	std::unique_ptr<BINDU::Sprite> bndryImg = std::make_unique<BINDU::Sprite>();
	bndryImg->SetTexture(boundaryTexture);
	bndryImg->setPosition(0.f + static_cast<float>(m_boundaryImgOffset), (TILE_SIZE.y * 5) - static_cast<float>(m_boundaryImgOffset));
	bndryImg->setSize((TILE_SIZE.x * 20.f) - 2.f * static_cast<float>( m_boundaryImgOffset), TILE_SIZE.y * 40.f);
	bndryImg->setActive(true);
	m_boundaryImg = bndryImg.get();

	std::unique_ptr<Paddle> paddle = std::make_unique<Paddle>();
	paddle->Init();
	paddle->onLoadResource();
	paddle->setPosition(400, 600);
	paddle->setSize(60, 16);
	paddle->setCollideAble(true);
	paddle->setActive(true);
	m_paddle = paddle.get();

	std::unique_ptr<BINDU::Layer> bgLayer = std::make_unique<BINDU::Layer>();
	bgLayer->setActive(false);
	bgLayer->AddObject(std::move(bgImg), "bgimg");
	bgLayer->setParallaxFactor({ 0.1f,0.1f });
	m_bgLayer1 = bgLayer.get();

	AddLayer(std::move(bgLayer), "bglayer");

	m_mapParser.setResourcePath(RelativeResourcePath("Resource/").c_str());


	int lvlCounter = 1;
	while (1)
	{

		tinyxml2::XMLDocument doc;
		std::string lvlPath = std::string("Resource/levels/level" + std::to_string(lvlCounter) + ".xml");
		doc.LoadFile(RelativeResourcePath(lvlPath.c_str()).c_str());

		if (doc.Error())
			break;

		m_mapParser.Load(RelativeResourcePath(lvlPath.c_str()).c_str(), *this);
		lvlCounter++;
	}

	m_switchLevel = true;

	std::unique_ptr<BINDU::Layer> fgLayer = std::make_unique<BINDU::Layer>();
	fgLayer->setActive(true);
	fgLayer->AddObject(std::move(bndryImg), "bndryimg");
	fgLayer->AddObject(std::move(paddle), "paddle");
	fgLayer->setParallaxFactor({ 0.0f,0.0f });
	m_fgLayer = fgLayer.get();

	AddLayer(std::move(fgLayer), "fglayer");


	m_font.LoadBitmapFont(RelativeResourcePath("Resource/unispace-bitmapfont.png").c_str());
	m_font.LoadWidthData(RelativeResourcePath("Resource/unispace-fontwidth.dat").c_str());
	m_font.setCharSize(10, 18);

	Scene::onLoadResource();

	for(auto& m:m_layers)
	{
		if(m->getGuid() != "bglayer" && m->getGuid() != "fglayer" && m.get() != m_tileLayer)
		{
			m->setActive(false);
		}
		if (BINDU::TileLayer* tileLayer = dynamic_cast<BINDU::TileLayer*>(m.get()))
			++m_totalLevel;
		
	}
	SetParticleEmitter();
}


void MainScene::Update(float dt)
{
	Scene::Update(dt);

	//TODO: Add level switching

	if (m_switchLevel)
	{
		SwitchLevel();
	}


	if (m_transitionPhase)
	{
		TransitionNext();
	}


	if(m_pauseMode)
	{
		Pause();
	}

	m_camera.Update(dt, m_circle.center);

	m_font.PrintText(0, 0, "Real: " + std::to_string(g_engine->getRealFrameRate()));
	m_font.PrintText(0, 24, "Core: " + std::to_string(g_engine->getCoreFrameRate()));

	BINDU::Vec2f normal;

	float  overlap{};

	if (circleVel.x > 300)
		circleVel.x = 300;
	else if (circleVel.x < -300)
		circleVel.x = -300;
	if (circleVel.y > 300)
		circleVel.y = 300;
	else if (circleVel.y < -300)
		circleVel.y = -300;

	m_circle.center += circleVel * dt;
	m_particleEmitter->setPosition(400,300);

	if (m_paddle->getPosition().x <= m_boundaryImg->getPosition().x + 3 * m_boundaryImgOffset)
	{
		m_paddle->setVelocity({ 0.f,0.f });
		m_paddle->setX(m_paddle->getX() + 2.f);
	}
	if (m_paddle->getPosition().x + m_paddle->getSize().width >= m_boundaryImg->getPosition().x + m_boundaryImg->getWidth() - 3 * m_boundaryImgOffset)
	{
		m_paddle->setVelocity({ 0.f,0.f });
		m_paddle->setX(m_paddle->getX() - 2.f);
	}

	if (m_circle.center.x - m_circle.radius <= m_boundaryImg->getPosition().x + 3 * m_boundaryImgOffset)
	{
		m_circle.center.x = m_circle.center.x + 2.f;
		circleVel.x += 5.f;
		circleVel.x = -circleVel.x;
	}
	if (m_circle.center.x + m_circle.radius >= m_boundaryImg->getPosition().x + m_boundaryImg->getWidth() - 3 * m_boundaryImgOffset)
	{
		m_circle.center.x = m_circle.center.x - 2.f;
		circleVel.x += 5.f;
		circleVel.x = -circleVel.x;
	}
	if (m_circle.center.y - m_circle.radius <= m_boundaryImg->getPosition().y + 3 * m_boundaryImgOffset)
	{
		m_circle.center.y = m_circle.center.y + 2.f;
		circleVel.y += 5.f;
		circleVel.y = -circleVel.y;
	}
	if (m_circle.center.y + m_circle.radius >= m_boundaryImg->getPosition().y + m_boundaryImg->getHeight() - 2 * m_boundaryImgOffset)
	{
		m_gameOver = true;
		
		//m_circle.center.y = m_circle.center.y - 2.f;
		//circleVel.y = -circleVel.y;
	}

	if(m_gameOver == true)
	{
		m_font.PrintText(300, 300, "GAME OVER", { 255,0,0,255 },1);
		m_font.PrintText(300, 350, "Press 'P' to start", { 255,0,0,255 }, 1);
		circleVel = { 100.f,100.f };
		m_circle.center = { 300.f,500.f };
		m_score = 0;
	}


	int currCount = 0;
	for(auto& m: m_tileLayer->getTileMap())
	{
		if (m)
		{
			if (BINDU::CollisionHandler::CircleVsRect(m_circle, circleVel, { m->position.x,m->position.y,m->size.width,m->size.height }, dt, normal, overlap))
			{
				m_circle.center -= normal * overlap;

				if (normal.y == 1.0f || normal.y == -1.0f)
				{
					circleVel.y += 10.f * normal.y;
					circleVel.y = -circleVel.y;
				}
				else if (normal.x == 1.0f || normal.x == -1.0f)
				{
					circleVel.x += 10.f * normal.x;
					circleVel.x = -circleVel.x;
				}
					


				BINDU::Bnd_Rect_F srcRect;

				if (m->id == 8)
				{
					m->id = 9;
					m->animator.setCurrentFrame(10);
					srcRect = m->animator.getCurrentFrame();
					m->sprite.setRect(srcRect);
				}
				else if (m->id == 9)
				{
					m->id = 10;
					m->animator.setCurrentFrame(11);
					srcRect = m->animator.getCurrentFrame();
					m->sprite.setRect(srcRect);
				}
				else
					m->sprite.setOpacity(m->sprite.getOpacity() - 50.f);

			}

			if(m && m->sprite.getOpacity()<250.f)
			{
				m->position.y = m->position.y + 100.f * dt;
				m->sprite.setOpacity(m->sprite.getOpacity() * 0.96f);

				if (m->sprite.getOpacity() < 50.f)
				{
					m.reset();
					m_score++;
					if (m_score > m_highScore)
						m_highScore = m_score;
				}
			}

		}
		if (m && m->id != 11 && m->id != 8 && m->id != 9 && m->id != 10)
			++currCount;
	}
	if(currCount == 0)
	{
		m_switchLevel = true;

		if (m_currentLevel >= m_layers.size())
			m_currentLevel = 0;

	}

	if (BINDU::CollisionHandler::CircleVsRect(m_circle, circleVel, m_paddle->getCollider(), dt, normal, overlap))
	{

		m_circle.center -= normal * overlap;

		if (normal.y == 1.0f || normal.y == -1.0f)
			circleVel.y = -circleVel.y;
		else if (normal.x == 1.0f || normal.x == -1.0f)
			circleVel.x = -circleVel.x;

		if (m_circle.center.x > m_paddle->getPosition().x + m_paddle->getSize().width / 2.f)
			circleVel.x += 20.f;
		if (m_circle.center.x < m_paddle->getPosition().x + m_paddle->getSize().width / 2.f)
			circleVel.x -= 20.f;

		circleVel += m_paddle->getVelocity() * 0.50f;

	}

	m_font.PrintText(640, 300, "SCORE: " + std::to_string(m_score));
	m_font.PrintText(640, 260, "HIGH SCORE: " + std::to_string(m_highScore));
}

void MainScene::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{

	Scene::Draw(graphics, m_camera.getCameraMatrix());



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


	graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Red));

	graphics->getRenderTarget()->FillEllipse(D2D1::Ellipse({ m_circle.center.x,m_circle.center.y }, m_circle.radius, m_circle.radius), graphics->getSolidColorBrush());

	//graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

	m_font.Draw(graphics, D2D1::IdentityMatrix());



}

void MainScene::ProcessInput()
{
	Scene::ProcessInput();

	if (BINDU::Input::isKeyPressed(BND_F))
		GameManager::SetState(State::MENU);

	if(BINDU::Input::isKeyPressed(BND_Y))
	{
		m_switchLevel = true;

		if (m_currentLevel >= m_layers.size())
			m_currentLevel = 0;

	}

	if(BINDU::Input::isKeyPressed(BND_P))
	{
		m_gameOver = false;
		m_pauseMode = false;
	}


	m_mouseCell = { (BINDU::Input::getMousePosition().x),(BINDU::Input::getMousePosition().y) };


	if (BINDU::Input::isMouseButtonHold(BND_BTN_LEFT))
	{
		m_circle.center = m_mouseCell;
	}

	if (BINDU::Input::isKeyPressed(BND_R))
		ResetMap();

}

void MainScene::ResetMap()
{
	for (auto& m : m_layers)
	{
		if (BINDU::TileLayer* tileLayer = dynamic_cast<BINDU::TileLayer*>(m.get()))
			tileLayer->onResetTilemap();
	}

}

void MainScene::GameComplete()
{

}

void MainScene::SwitchLevel()
{
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

	if (m_tileLayer)
	{
		for (auto& m : m_layers)
		{
			if (m.get() != m_tileLayer && m->getGuid() != "fglayer" && m->getGuid() != "bglayer")
			{
				m->setActive(false);
			}
		}

		m_tileLayer->setActive(true);
		m_tileLayer->onResetTilemap();
		m_tileLayer->setParallaxFactor({ 0.0f,0.0f });

		m_switchLevel = false;
		m_pauseMode = true;
		m_transitionPhase = true;

		for (auto& m : m_tileLayer->getTileMap())
		{
			if (m)
				m->sprite.setScale(0.1f, 0.1f);
		}

	}
}

void MainScene::TransitionNext()
{
	for (auto& m : m_tileLayer->getTileMap())
	{
		if (m)
		{
			m->sprite.setScale(m->sprite.getScale() * 1.1f);
			if (m->sprite.getScale().x >= 1.0)
			{
				m_transitionPhase = true;
				m->sprite.setScale(1.f, 1.f);
				m_transitionPhase = false;
			}
		}
	}
}

void MainScene::Pause()
{
	m_font.PrintText(350, 350, "LEVEL: " + std::to_string(m_currentLevel), { 255,0,0,255 });
	m_font.PrintText(300, 450, "Press 'P' to start", { 255,0,0,255 });
	m_circle.center = { 300.f,500.f };
}

void MainScene::SetParticleEmitter()
{

	BINDU::ParticleProps props;
	props.colorOpacityRange = { 255,255 };
	props.minTimetoChangeColor = 10;
	
	props.size = { 50.f,50.f };
	props.velocity = BINDU::Vec2f(BINDU::RandomNumber::Get(2, 10), BINDU::RandomNumber::Get(2, 10));
	props.rotation = 0;
	props.startColor = { 255,0,0,255 };
	props.endColor = { 255,0,0,255 };
	props.startScale = 1.0f ;
	props.endScale = 1.0f;
	props.fadeOut = false;
	props.lifeTime = 10000;

	std::unique_ptr<BINDU::ParticleEmitter> emitter = std::make_unique<BINDU::ParticleEmitter>();

	BINDU::Texture tex(RelativeResourcePath("Resource/tree-top.png").c_str());

	emitter->Init(props);
	emitter->setSize(10, 10);
	
	emitter->onLoop(true);
	emitter->setDirection(0);
	emitter->setPosition(m_circle.center);
	emitter->setMax(500);
	emitter->SetTexture(tex);
	emitter->setSpread(20);
	emitter->setEmissionInterval(1);
	emitter->setEmissionRate(1);
	emitter->setActive(true);
	m_particleEmitter = emitter.get();
	
	m_fgLayer->AddObject(std::move(emitter), "emitter");

}
