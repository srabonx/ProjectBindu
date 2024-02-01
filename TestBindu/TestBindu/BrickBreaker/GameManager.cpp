#include "GameManager.h"

void GameManager::Preload()
{
	BINDU::EngineProperties props;
	props.versionNotice = true;
	props.windowTitle = "Brick Breaker";
	props.windowWidth = m_worldSize.width;
	props.windowHeight = m_worldSize.height;

	g_engine->setEngineProperties(props);
}

void GameManager::Init()
{
	m_mainScene = std::make_unique<MainScene>();

	m_mainScene->Init();
	m_font.Init();
	
}

void GameManager::onLoadResource()
{
	m_mainScene->onLoadResource(m_worldSize);
	m_font.LoadBitmapFont("BrickBreaker/Resource/unispace-bitmapfont.png");
	m_font.LoadWidthData("BrickBreaker/Resource/unispace-fontwidth.dat");
	m_font.setCharSize(24, 20);
}

void GameManager::Update(float dt)
{
	m_mainScene->ProcessInput();


	m_mainScene->Update(dt);

//	m_font.PrintText(0, 0, "Real: " + std::to_string(g_engine->getRealFrameRate()));
//	m_font.PrintText(0, 24, "Core: " + std::to_string(g_engine->getCoreFrameRate()));

}

void GameManager::Draw(BINDU::Graphics* graphics)
{
	graphics->getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	

	m_mainScene->Draw(graphics, D2D1::Matrix3x2F::Identity());

//	m_font.Draw(graphics, D2D1::Matrix3x2F::Identity());

	//graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));

	//for (int x = TILE_SIZE.x; x <= m_worldSize.width; x+= TILE_SIZE.x)
	//{
	//		graphics->getRenderTarget()->DrawLine({ static_cast<float>(x),static_cast<float>(0)},
	//			{ static_cast<float>(x),static_cast<float>(m_worldSize.height) },
	//			graphics->getSolidColorBrush());
	//}

	//for (int y = TILE_SIZE.y; y <= m_worldSize.height; y += TILE_SIZE.y)
	//{
	//	graphics->getRenderTarget()->DrawLine({ 0.f,static_cast<float>(y) },
	//		{ static_cast<float>(m_worldSize.width),static_cast<float>(y) },
	//		graphics->getSolidColorBrush());
	//}

}
