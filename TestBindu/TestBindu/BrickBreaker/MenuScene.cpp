#include "MenuScene.h"

#include "CollisionHandler.h"
#include "GameManager.h"
#include "Constants.h"
#include "Input.h"

void MenuScene::Init()
{
	m_movingFont.Init();
	m_font.Init();
}

void MenuScene::onLoadResource()
{
	Scene::onLoadResource();

	std::unique_ptr<BINDU::Layer> fgLayer = std::make_unique<BINDU::Layer>();
	fgLayer->setActive(true);
	m_fgLayer = fgLayer.get();

	std::unique_ptr<BINDU::Layer> bgLayer = std::make_unique<BINDU::Layer>();
	bgLayer->setActive(true);
	m_bgLayer = bgLayer.get();

	m_buttonTexture.LoadFromFile(RelativeResourcePath("Resource/images/buttons.png").c_str());
	m_selectorTexture.LoadFromFile(RelativeResourcePath("Resource/images/selector2.png").c_str());
	m_bgTexture.LoadFromFile(RelativeResourcePath("Resource/images/star.png").c_str());

	std::unique_ptr<BINDU::Sprite> bgImage = std::make_unique<BINDU::Sprite>();
	bgImage->SetTexture(m_bgTexture);
	bgImage->setPosition(0,0);
	bgImage->setSize(800, 800);
	bgImage->setActive(true);
	bgImage->setOrigin(800 / 2, 720 / 2);
	bgImage->setScale(0.5f, 0.5f);
	m_bgImage = bgImage.get();

	BINDU::Texture tex(RelativeResourcePath("Resource/images/starfield.png").c_str());
	std::unique_ptr<BINDU::Sprite> starBgImage = std::make_unique<BINDU::Sprite>();
	starBgImage->SetTexture(tex);
	starBgImage->setPosition(-200, -200);
	starBgImage->setSize(1224, 1024);
	starBgImage->setActive(true);
	

	std::unique_ptr<BINDU::Layer> bgLayer2 = std::make_unique<BINDU::Layer>();
	bgLayer2->setActive(true);
	bgLayer2->setParallaxFactor({ 0.5f,0.5f });

	bgLayer2->AddObject(std::move(starBgImage), "sbgimage");

	m_bgLayer->AddObject(std::move(bgImage), "bgimage");

	std::unique_ptr<BINDU::Sprite> playButton = std::make_unique<BINDU::Sprite>();
	playButton->SetTexture(m_buttonTexture);
	playButton->setPosition(200, 100);
	playButton->setSize(96, 32);
	playButton->setRect({ 0.f,0.f * 32.f,96,32 });
	playButton->setCollider({ 0.f,0.f,playButton->getSize().width,playButton->getSize().height });
	playButton->setActive(true);

	m_playButton = playButton.get();

	m_playButtonAnimator.setTotalColumn(4);
	m_playButtonAnimator.setTotalFrame(4);
	m_playButtonAnimator.setFrameTime(100);
	m_playButtonAnimator.setCurrentRow(1);
	m_playButtonAnimator.setFrameSize(96.f, 32.f);

	m_fgLayer->AddObject(std::move(playButton), "playbutton");

	std::unique_ptr<BINDU::Sprite> optionButton = std::make_unique<BINDU::Sprite>();
	optionButton->SetTexture(m_buttonTexture);
	optionButton->setPosition(200, 140);
	optionButton->setSize(96, 32);
	optionButton->setRect({ 0.f,1.f * 32.f,96,32 });
	optionButton->setCollider({ 0.f,0.f,optionButton->getSize().width,optionButton->getSize().height });
	optionButton->setActive(true);

	m_optionButton = optionButton.get();

	m_optionButtonAnimator.setTotalColumn(4);
	m_optionButtonAnimator.setTotalFrame(4);
	m_optionButtonAnimator.setFrameTime(100);
	m_optionButtonAnimator.setCurrentRow(2);
	m_optionButtonAnimator.setFrameSize(96.f, 32.f);

	m_fgLayer->AddObject(std::move(optionButton), "optionbutton");

	std::unique_ptr<BINDU::Sprite> exitButton = std::make_unique<BINDU::Sprite>();
	exitButton->SetTexture(m_buttonTexture);
	exitButton->setPosition(200, 180);
	exitButton->setSize(96, 32);
	exitButton->setRect({ 0.f,4.f * 32.2f,96,32 });
	exitButton->setCollider({ 0.f,0.f,exitButton->getSize().width,exitButton->getSize().height });
	exitButton->setActive(true);

	m_exitButton = exitButton.get();

	m_exitButtonAnimator.setTotalColumn(4);
	m_exitButtonAnimator.setTotalFrame(4);
	m_exitButtonAnimator.setFrameTime(100);
	m_exitButtonAnimator.setCurrentRow(5);
	m_exitButtonAnimator.setFrameSize(96.f, 32.f);

	m_fgLayer->AddObject(std::move(exitButton), "exitbutton");

	


	std::unique_ptr<BINDU::Sprite> selector = std::make_unique<BINDU::Sprite>();

	selector->SetTexture(m_selectorTexture);
	selector->setPosition(200, 100);
	selector->setSize(96, 32);
	selector->setCollider({ 0.0f, 0.0f, selector->getSize().width, selector->getSize().height });
	selector->setActive(true);

	m_selectorAnimator.setTotalColumn(4);
	m_selectorAnimator.setTotalFrame(4);
	m_selectorAnimator.setCurrentRow(0);
	m_selectorAnimator.setFrameSize(96, 32);
	m_selectorAnimator.setFrameTime(200);

	m_selector = selector.get();

	fgLayer->AddObject(std::move(selector), "selector");


	m_fgLayer->setOffsetX(150);
	m_fgLayer->setOffsetY(100);
//	m_fgLayer->showDebug(true);
	m_fgLayer->setParallaxFactor({ 0.0f, 0.0f });

	m_bgLayer->setParallaxFactor({ 0.7f,0.7f });

	AddLayer(std::move(bgLayer2), "bglayer2");

	SetParticleEmitter();

	std::unique_ptr<BINDU::Layer> optionLayer = std::make_unique<BINDU::Layer>();
	optionLayer->setActive(false);
	optionLayer->setOffsetX(150);
	optionLayer->setOffsetY(100);
	optionLayer->setParallaxFactor({ 0.0f,0.0f });
	m_optionLayer = optionLayer.get();

	std::unique_ptr<BINDU::Sprite> musicOnButton = std::make_unique<BINDU::Sprite>();
	musicOnButton->setPosition(220.f, 150.f);
	musicOnButton->setSize(96.f, 32.f);
	musicOnButton->SetTexture(m_buttonTexture);
	musicOnButton->setCollider({ 32.f,4.f,32.f,28.f });
	musicOnButton->setActive(true);
	musicOnButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });

	m_musicOnButton = musicOnButton.get();

	std::unique_ptr<BINDU::Sprite> musicOffButton = std::make_unique<BINDU::Sprite>();
	musicOffButton->setPosition(220.f, 150.f);
	musicOffButton->setSize(96.f, 32.f);
	musicOffButton->SetTexture(m_buttonTexture);
	musicOffButton->setCollider({ 32.f,4.f,32.f,28.f });
	musicOffButton->setActive(false);
	musicOffButton->setRect({ 96.f,5.f * 32.f,96.f,32.f });

	m_musicOffButton = musicOffButton.get();


	std::unique_ptr<BINDU::Sprite> sfxOnButton = std::make_unique<BINDU::Sprite>();
	sfxOnButton->setPosition(220.f, 200.f);
	sfxOnButton->setSize(96.f, 32.f);
	sfxOnButton->SetTexture(m_buttonTexture);
	sfxOnButton->setCollider({ 32.f,4.f,32.f,28.f });
	sfxOnButton->setActive(true);
	sfxOnButton->setRect({ 0.f,5.f * 32.f,96.f,32.f });

	m_sfxOnButton = sfxOnButton.get();

	std::unique_ptr<BINDU::Sprite> sfxOffButton = std::make_unique<BINDU::Sprite>();
	sfxOffButton->setPosition(220.f, 200.f);
	sfxOffButton->setSize(96.f, 32.f);
	sfxOffButton->SetTexture(m_buttonTexture);
	sfxOffButton->setCollider({ 32.f,4.f,32.f,28.f });
	sfxOffButton->setActive(false);
	sfxOffButton->setRect({ 96.f,5.f * 32.f,96.f,32.f });

	m_sfxOffButton = sfxOffButton.get();

	std::unique_ptr<BINDU::Sprite> backButton = std::make_unique<BINDU::Sprite>();
	backButton->setPosition(220.f, 100.f);
	backButton->setSize(96.f, 32.f);
	backButton->SetTexture(m_buttonTexture);
	backButton->setCollider({ 32.f,4.f,32.f,28.f });
	backButton->setActive(true);
	backButton->setRect({ 192.f,5.f * 32.f,96.f,32.f });

	m_backButton = backButton.get();


	m_optionLayer->AddObject(std::move(backButton), "backbutton");
	m_optionLayer->AddObject(std::move(musicOnButton), "musiconbutton");
	m_optionLayer->AddObject(std::move(musicOffButton), "musicoffbutton");
	m_optionLayer->AddObject(std::move(sfxOnButton), "sfxonbutton");
	m_optionLayer->AddObject(std::move(sfxOffButton), "sfxoffbutton");


	AddLayer(std::move(bgLayer), "bglayer");
	AddLayer(std::move(fgLayer), "fglayer");
	AddLayer(std::move(optionLayer), "optionlayer");

	m_font.LoadBitmapFont(RelativeResourcePath("Resource/images/unispace-bitmapfont.png").c_str());
	m_font.LoadWidthData(RelativeResourcePath("Resource/images/unispace-fontwidth.dat").c_str());
	m_font.setCharSize(15, 20);
	
	m_movingFont.LoadBitmapFont(RelativeResourcePath("Resource/images/unispace-bitmapfont.png").c_str());
	m_movingFont.LoadWidthData(RelativeResourcePath("Resource/images/unispace-fontwidth.dat").c_str());
	m_movingFont.setCharSize(15, 20);

	m_fontPos.y = 700.f;
	m_fontPos.x = 500.f;

	m_camera.setTopLeft(-300, -300);
	m_camera.setBottomRight(1200, 1200);
	m_camera.setViewSize(800, 720);

}

void MenuScene::onReleaseResource()
{
	Scene::onReleaseResource();
}

void MenuScene::Update(float dt)
{


	Scene::Update(dt);

//	m_optionButtonAnimator.Animate(src);
//	m_exitButtonAnimator.Animate(src);

	BINDU::Vec2f mousePos = BINDU::Input::getMousePosition() - BINDU::Vec2f{ m_fgLayer->getOffsetX(), m_fgLayer->getOffsetY()};

	m_camera.Update(dt, mousePos);


	if (BINDU::CollisionHandler::PointVsRect(mousePos, m_playButton->getRectCollider()))
	{
		m_selector->setY(m_playButton->getY());
	}
	else if (BINDU::CollisionHandler::PointVsRect(mousePos, m_optionButton->getRectCollider()))
	{
		m_selector->setY(m_optionButton->getY());
	}
	else if (BINDU::CollisionHandler::PointVsRect(mousePos, m_exitButton->getRectCollider()))
	{
		m_selector->setY(m_exitButton->getY());
	}


	if (m_selector->getY() > m_exitButton->getY())
		m_selector->setY(m_playButton->getY());
	else if(m_selector->getY() < m_playButton->getY())
		m_selector->setY(m_exitButton->getY());

	if(m_switchToGame)
	{
		m_playButton->setRect(m_playButtonAnimator.Animate(2, 4));

		if(m_playButtonAnimator.getCurrentFrameNum() == 4)
		{
			m_switchToGame = false;
			GameManager::getSoundSystem()->Stop("menumusic");
			GameManager::SetState(GameState::GAME);
		}
	}
	else if(m_switchToOption)
	{
		m_optionButton->setRect(m_optionButtonAnimator.Animate(2, 4));

		if(m_optionButtonAnimator.getCurrentFrameNum() == 4)
		{
			m_switchToOption = false;
			// TODO::ADD option
			m_optionLayer->setActive(true);
			m_fgLayer->setActive(false);

		}
	}
	else if(m_switchToQuit)
	{
		m_exitButton->setRect(m_exitButtonAnimator.Animate(2, 4));

		if(m_exitButtonAnimator.getCurrentFrameNum() == 4)
		{
			PostQuitMessage(0);
		}
	}

	if(m_optionLayer->isActive())
	{
		if(GameManager::getMusic())
		{
			m_musicOffButton->setActive(false);
			m_musicOnButton->setActive(true);
		}
		else
		{
			m_musicOffButton->setActive(true);
			m_musicOnButton->setActive(false);
		}

		if(GameManager::getSfx())
		{
			m_sfxOffButton->setActive(false);
			m_sfxOnButton->setActive(true);
		}
		else
		{
			m_sfxOffButton->setActive(true);
			m_sfxOnButton->setActive(false);
		}

		m_font.PrintText(350, 150, "OPTION", { 245.f,53.f,170.f,255.f });
		m_font.PrintText(320, 210, "BACK ", { 245.f,53.f,170.f,255.f });
		m_font.PrintText(320, 260, "MUSIC " ,{ 245.f,53.f,170.f,255.f});
		m_font.PrintText(320, 310, "SFX ", { 245.f,53.f,170.f,255.f });
	}

	UpdateMovingFont(dt);
}

void MenuScene::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{
	BINDU::Bnd_Rect_F srcRect;
	m_selectorAnimator.Animate(srcRect);

	m_selector->setRect(srcRect);

	Scene::Draw(graphics, m_camera.getCameraMatrix());

	if (m_optionLayer->isActive())
		m_font.Draw(graphics, cameraMatrix);

	m_movingFont.Draw(graphics, m_camera.getCameraMatrix());
}

void MenuScene::ProcessInput()
{
	Scene::ProcessInput();

	if (m_fgLayer->isActive())
	{
		if (BINDU::Input::isKeyPressed(BND_S) || BINDU::Input::isKeyPressed(BND_DOWN))
		{
			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("navsound", false, 1.0f);
			m_selector->setY(m_selector->getY() + 40);
		}

		if (BINDU::Input::isKeyPressed(BND_W) || BINDU::Input::isKeyPressed(BND_UP))
		{
			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("navsound", false, 1.0f);
			m_selector->setY(m_selector->getY() - 40);
		}

		if (BINDU::Input::isKeyPressed(BND_ENTER))
		{
			if (m_selector->getY() == m_playButton->getY())
			{
				m_switchToGame = true;
			}
			else if (m_selector->getY() == m_optionButton->getY())
			{
				m_switchToOption = true;
			}
			else if (m_selector->getY() == m_exitButton->getY())
			{
				m_switchToQuit = true;
			}

			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("clicksound", false, 1.0f);

		}

		if (BINDU::Input::isMouseButtonPressed(BND_BTN_LEFT))
		{
			if (m_selector->getY() == m_playButton->getY())
			{
				m_switchToGame = true;
			}
			else if (m_selector->getY() == m_optionButton->getY())
			{
				m_switchToOption = true;
			}
			else if (m_selector->getY() == m_exitButton->getY())
			{
				m_switchToQuit = true;
			}
			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("clicksound", false);
		}
	}
	else if(m_optionLayer->isActive())
	{
		if(BINDU::Input::isMouseButtonPressed(BND_BTN_LEFT))
		{
			if (m_musicOnButton->isActive() && BINDU::CollisionHandler::PointVsRect(BINDU::Input::getMousePosition() - BINDU::Vec2f(150.f,100.f), m_musicOnButton->getRectCollider()))
			{
				m_musicOnButton->setActive(false);
				m_musicOffButton->setActive(true);

				GameManager::setMusic(false);

				GameManager::getSoundSystem()->Stop("menumusic");
				GameManager::getSoundSystem()->Stop("gamemusic");
			}
			else if (m_musicOffButton->isActive() && BINDU::CollisionHandler::PointVsRect(BINDU::Input::getMousePosition() - BINDU::Vec2f(150.f, 100.f), m_musicOffButton->getRectCollider()))
			{
				m_musicOnButton->setActive(true);
				m_musicOffButton->setActive(false);

				GameManager::setMusic(true);

				GameManager::getSoundSystem()->Play("menumusic");
			}
			else if (m_sfxOnButton->isActive() && BINDU::CollisionHandler::PointVsRect(BINDU::Input::getMousePosition() - BINDU::Vec2f(150.f, 100.f), m_sfxOnButton->getRectCollider()))
			{
				m_sfxOnButton->setActive(false);
				m_sfxOffButton->setActive(true);

				GameManager::setSfx(false);

			}
			else if (m_sfxOffButton->isActive() && BINDU::CollisionHandler::PointVsRect(BINDU::Input::getMousePosition() - BINDU::Vec2f(150.f, 100.f), m_sfxOffButton->getRectCollider()))
			{
				m_sfxOnButton->setActive(true);
				m_sfxOffButton->setActive(false);

				GameManager::setSfx(true);
			}
			else if (m_backButton->isActive() && BINDU::CollisionHandler::PointVsRect(BINDU::Input::getMousePosition() - BINDU::Vec2f(150.f, 100.f), m_backButton->getRectCollider()))
			{

				m_optionLayer->setActive(false);
				m_fgLayer->setActive(true);

			}


			if (GameManager::getSfx())
				GameManager::getSoundSystem()->Play("clicksound", false);
		}
	}
}

void MenuScene::SetParticleEmitter()
{
	BINDU::ParticleProps props;

	props.size = { 15.f,15.f };
	props.velocity = { 5.f,5.f };
	props.startScale = 1.0f;
	props.endScale = 0.5f;
	props.startColor = { 255,255,255,500 };
	props.endColor = { 230,230,250,255 };
	//	props.colorRandomnessRange = BINDU::iRange(0, 255);
	props.colorOpacityRange = BINDU::iRange(255, 555);
	props.minTimetoChangeColor = 5.5f;
	props.fadeOut = true;
	props.rotation = 0;
	props.rotationRate = 20;
	props.lifeTime = 10.0f;

	std::unique_ptr<BINDU::ParticleEmitter> emitter = std::make_unique<BINDU::ParticleEmitter>();

	BINDU::Texture tex(RelativeResourcePath("Resource/images/star_03.png").c_str());

	emitter->Init(props);
	emitter->SetTexture(tex);
	emitter->setSize(1224, 1024);
	emitter->onLoop(true);
	emitter->setDirection(100);
	emitter->setPosition(-300.f, -300.f);
	emitter->setMax(1000);
	emitter->setSpread(180);
	emitter->setEmissionInterval(50);
	emitter->setEmissionRate(1);
	emitter->setActive(true);
	m_starEmitter = emitter.get();

	std::unique_ptr<BINDU::Layer> bglayer2 = std::make_unique<BINDU::Layer>();
	bglayer2->setActive(true);
	bglayer2->setParallaxFactor({ 0.8f,0.8f });
	m_starEmitterLayer = bglayer2.get();
	bglayer2->AddObject(std::move(emitter), "staremitter");

	AddLayer(std::move(bglayer2), "staremitterlayer");

}

void MenuScene::UpdateMovingFont(float dt)
{
	m_fontPos.x -= 100.f * dt;

	const std::string str1 = "BrickBreaker v1.0.0 - Made using : ";
	const std::string str2 = "Artworks By: ";
	const std::string str3 = "Aitneony";

	if (m_fontPos.x <= m_camera.getTopLeft().x - (str1.length() * 10.0f))
		m_fontPos.x = m_camera.getBottomRight().x;
	

	m_movingFont.PrintText(static_cast<int>(m_fontPos.x), static_cast<int>(m_fontPos.y), str1);
	m_movingFont.PrintText(static_cast<int>(m_fontPos.x + str1.length() * 8.f), static_cast<int>(m_fontPos.y), g_engine->getVersionText(),{0.f,0.f,255.f,255.f});
	m_movingFont.PrintText(static_cast<int>(m_fontPos.x), static_cast<int>(m_fontPos.y + 40.f), str2);
	m_movingFont.PrintText(static_cast<int>(m_fontPos.x + str2.length() * 8.0f), static_cast<int>(m_fontPos.y + 40.f), str3, { 255.f,0.f,0.f,255.f });
}
