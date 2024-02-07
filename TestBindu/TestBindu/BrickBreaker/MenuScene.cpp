#include "MenuScene.h"

#include "CollisionHandler.h"
#include "GameManager.h"
#include "Constants.h"
#include "Input.h"

void MenuScene::Init()
{

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

	m_buttonTexture.LoadFromFile(RelativeResourcePath("Resource/Spritesheet_UI_Flat_Button_02.png").c_str());
	m_selectorTexture.LoadFromFile(RelativeResourcePath("Resource/selector.png").c_str());
	m_bgTexture.LoadFromFile(RelativeResourcePath("Resource/star.png").c_str());

	std::unique_ptr<BINDU::Sprite> bgImage = std::make_unique<BINDU::Sprite>();
	bgImage->SetTexture(m_bgTexture);
	bgImage->setPosition(0,0);
	bgImage->setSize(800, 800);
	bgImage->setActive(true);
	bgImage->setOrigin(800 / 2, 800 / 2);
	bgImage->setScale(0.5f, 0.5f);
	m_bgImage = bgImage.get();

	BINDU::Texture tex(RelativeResourcePath("Resource/starbg2.jpg").c_str());
	std::unique_ptr<BINDU::Sprite> starBgImage = std::make_unique<BINDU::Sprite>();
	starBgImage->SetTexture(tex);
	starBgImage->setPosition(-100, -100);
	starBgImage->setSize(1024, 1024);
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
	playButton->setRect({ 0.f,6.f * 32.f,96,32 });
	playButton->setActive(true);

	m_playButton = playButton.get();

	m_fgLayer->AddObject(std::move(playButton), "playbutton");

	std::unique_ptr<BINDU::Sprite> optionButton = std::make_unique<BINDU::Sprite>();
	optionButton->SetTexture(m_buttonTexture);
	optionButton->setPosition(200, 140);
	optionButton->setSize(96, 32);
	optionButton->setRect({ 0.f,6.f * 32.f,96,32 });
	optionButton->setActive(true);

	m_optionButton = optionButton.get();

	m_fgLayer->AddObject(std::move(optionButton), "optionbutton");

	std::unique_ptr<BINDU::Sprite> exitButton = std::make_unique<BINDU::Sprite>();
	exitButton->SetTexture(m_buttonTexture);
	exitButton->setPosition(200, 180);
	exitButton->setSize(96, 32);
	exitButton->setRect({ 0.f,6.f * 32.f,96,32 });
	exitButton->setActive(true);

	m_exitButton = exitButton.get();

	m_fgLayer->AddObject(std::move(exitButton), "exitbutton");


	std::unique_ptr<BINDU::Sprite> selector = std::make_unique<BINDU::Sprite>();

	selector->SetTexture(m_selectorTexture);
	selector->setPosition(200, 100);
	selector->setSize(96, 32);
	selector->setActive(true);

	m_animator.setTotalColumn(4);
	m_animator.setTotalFrame(4);
	m_animator.setCurrentRow(1);
	m_animator.setFrameSize(96, 32);
	m_animator.setFrameTime(200);

	m_selector = selector.get();

	fgLayer->AddObject(std::move(selector), "selector");


	m_fgLayer->setOffsetX(150);
	m_fgLayer->setOffsetY(100);
//	m_fgLayer->showDebug(true);
	m_fgLayer->setParallaxFactor({ 0.0f, 0.0f });

	m_bgLayer->setParallaxFactor({ 0.7f,0.7f });

	AddLayer(std::move(bgLayer2), "bglayer2");
	AddLayer(std::move(bgLayer), "bglayer");
	AddLayer(std::move(fgLayer), "fglayer");

	m_camera.setTopLeft(-100, -100);
}

void MenuScene::Update(float dt)
{
	Scene::Update(dt);

	BINDU::Vec2f mousePos = BINDU::Input::getMousePosition() - BINDU::Vec2f{ m_fgLayer->getOffsetX(), m_fgLayer->getOffsetY()};

	m_camera.Update(dt, mousePos);

	if(BINDU::CollisionHandler::PointVsRect(mousePos,m_playButton->getCollider()))
		m_selector->setY(m_playButton->getY());
	if(BINDU::CollisionHandler::PointVsRect(mousePos,m_optionButton->getCollider()))
		m_selector->setY(m_optionButton->getY());
	if(BINDU::CollisionHandler::PointVsRect(mousePos,m_exitButton->getCollider()))
		m_selector->setY(m_exitButton->getY());


	if (m_selector->getY() > m_exitButton->getY())
		m_selector->setY(m_playButton->getY());
	if(m_selector->getY() < m_playButton->getY())
		m_selector->setY(m_exitButton->getY());
}

void MenuScene::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{
	BINDU::Bnd_Rect_F srcRect;
	m_animator.Animate(srcRect);

	m_selector->setRect(srcRect);

	Scene::Draw(graphics, m_camera.getCameraMatrix());
}

void MenuScene::ProcessInput()
{
	Scene::ProcessInput();

	if (BINDU::Input::isKeyPressed(BND_S))
		m_selector->setY(m_selector->getY() + 40);

	if (BINDU::Input::isKeyPressed(BND_W))
		m_selector->setY(m_selector->getY() - 40);

	if(BINDU::Input::isKeyPressed(BND_F))
	{
		if(m_selector->getY() == m_playButton->getY())
		{
			GameManager::SetState(State::GAME);
		}

		if(m_selector->getY() == m_optionButton->getY())
		{
			//TODO:
		}
		if (m_selector->getY() == m_exitButton->getY())
		{
			PostQuitMessage(0);
		}
	}

	if(BINDU::Input::isMouseButtonPressed(BND_BTN_LEFT))
	{
		if (m_selector->getY() == m_playButton->getY())
		{
			GameManager::SetState(State::GAME);
		}

		if (m_selector->getY() == m_optionButton->getY())
		{
			//TODO:
		}
		if (m_selector->getY() == m_exitButton->getY())
		{
			PostQuitMessage(0);
		}
	}

}
