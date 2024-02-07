#include "GameManager.h"

#include "Input.h"

State GameManager::m_state = State::MENU;

void GameManager::Preload()
{
	BINDU::EngineProperties props;
	props.versionNotice = true;
	props.windowTitle = "Brick Breaker";
	props.windowWidth = m_worldSize.width;
	props.windowHeight = m_worldSize.height;
	props.windowIcon = "../Resource/images/application_icon.ico";

	g_engine->setEngineProperties(props);

	m_state = State::MENU;
}

void GameManager::Init()
{
	m_menuScene = std::make_unique<MenuScene>();
	m_menuScene->Init();
	m_menuScene->setActive(true);

	m_mainScene = std::make_unique<MainScene>();
	m_mainScene->Init();
	m_mainScene->setActive(false);


	
}

void GameManager::onLoadResource()
{
	m_mainScene->onLoadResource(m_worldSize);
	m_menuScene->onLoadResource();

	m_sceneManager.AddScene(std::move(m_menuScene), "menu");
	m_sceneManager.AddScene(std::move(m_mainScene), "main");
}

void GameManager::Update(float dt)
{
//	m_mainScene->ProcessInput();


	if(m_state == State::MENU)
	{
		m_sceneManager.getScene("main")->setActive(false);
		m_sceneManager.getScene("menu")->setActive(true);
	}
	else if(m_state == State::GAME)
	{
		m_sceneManager.getScene("main")->setActive(true);
		m_sceneManager.getScene("menu")->setActive(false);
	}


	m_sceneManager.ProcessInput();
	m_sceneManager.Update(dt);
//	m_mainScene->Update(dt);

}

void GameManager::Draw(BINDU::Graphics* graphics)
{
	graphics->getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	
	m_sceneManager.Draw(graphics, D2D1::Matrix3x2F::Identity());
	//m_mainScene->Draw(graphics, D2D1::Matrix3x2F::Identity());


}
