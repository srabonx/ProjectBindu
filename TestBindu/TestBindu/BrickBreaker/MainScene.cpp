#include "MainScene.h"

#include "Input.h"

void MainScene::Init()
{
	m_bgLayer = std::make_unique<BINDU::Layer>();
	m_fgLayer = std::make_unique<BINDU::Layer>();

	m_backgroundImg = std::make_unique<BINDU::Sprite>();
	m_boundaryImg = std::make_unique<BINDU::Sprite>();

	m_paddle = std::make_unique<Paddle>();

	m_font.Init();
}

void MainScene::onLoadResource(const BINDU::Bnd_Size& windSize)
{
	m_bgTexture.LoadFromFile("BrickBreaker/Resource/images/background.png");
	m_boundaryTexture.LoadFromFile("BrickBreaker/Resource/images/borders.png");

	m_backgroundImg->SetTexture(m_bgTexture);
	m_backgroundImg->setPosition(0, 0);
	m_backgroundImg->setSize(windSize.width,windSize.height);

	m_boundaryImg->SetTexture(m_boundaryTexture);
	m_boundaryImg->setPosition(0.0f + m_boundaryImgOffset, (TILE_SIZE.y * 5) - m_boundaryImgOffset);
	m_boundaryImg->setSize((TILE_SIZE.x * 20) - 2 * m_boundaryImgOffset , TILE_SIZE.y * 40);

	m_backgroundImg->setActive(true);
	m_boundaryImg->setActive(true);
	m_bgLayer->AddObject(std::move(m_backgroundImg), "bgimg");
	m_bgLayer->setParallaxFactor({ 0.7f, 0.7f });
//	m_bgLayer->AddObject(std::move(m_boundaryImg), "bndimg");
	m_bgLayer->setActive(true);

	m_paddle->onLoadResource();
	m_paddle->setPosition(TILE_SIZE.x * 10, TILE_SIZE.y * 37);
	m_paddle->setSize(60, 16);

	scene = std::make_unique<BINDU::Scene>();

	scene->setActive(true);

	scene->AddLayer(std::move(m_bgLayer), "bglayer");


	m_mapParser.setResourcePath("BrickBreaker/Resource/");

	m_mapParser.Load("BrickBreaker/Resource/level1.xml",*scene);

	tileLayer = dynamic_cast<BINDU::TileLayer*> (scene->getLayer("Tile Layer 1"));

//	scene->setWorldTileWidth(32);
//	scene->setWorldTileHeight(16);
	scene->onLoadResource();
	m_camera.SetTarget(m_paddle->getOrigin());

	m_font.LoadBitmapFont("BrickBreaker/Resource/unispace-bitmapfont.png");
	m_font.LoadWidthData("BrickBreaker/Resource/unispace-fontwidth.dat");
	m_font.setCharSize(24, 20);

}

void MainScene::Update(float dt)
{
	m_camera.Update(dt, m_paddle->getCenter());
	m_paddle->Update(dt);
	scene->Update(dt);

	m_font.PrintText(0, 0, "Real: " + std::to_string(g_engine->getRealFrameRate()));
	m_font.PrintText(0, 24, "Core: " + std::to_string(g_engine->getCoreFrameRate()));

}

void MainScene::Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
{
	

//	m_backgroundImg->Draw(graphics, m_camera.getCameraMatrix());
	m_boundaryImg->Draw(graphics, m_camera.getCameraMatrix());

	scene->Draw(graphics, m_camera.getCameraMatrix());
	m_paddle->Draw(graphics, m_camera.getCameraMatrix());


	// Drawing the grid
	graphics->getRenderTarget()->SetTransform(m_camera.getCameraMatrix());
	for (int y = static_cast<int>(m_boundaryImg->getPosition().y + (3 * m_boundaryImgOffset)); y <= m_boundaryImg->getSize().height ; y += TILE_SIZE.y)
	{
		for (int x = static_cast<int>(m_boundaryImg->getPosition().x + (3 * m_boundaryImgOffset)); x <= m_boundaryImg->getSize().width - (3 * m_boundaryImgOffset); x += TILE_SIZE.x)
		{
				
				graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::DarkSlateGray));
				graphics->getRenderTarget()->DrawRectangle({ static_cast<float>(x),static_cast<float>(y),static_cast<float>(x) + static_cast<float>(TILE_SIZE.x),static_cast<float>(y) + static_cast<float>(TILE_SIZE.y) }, graphics->getSolidColorBrush());
		}

	}
	graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());

	if (BINDU::Input::isMouseButtonHold(BND_BTN_LEFT))
	{
		m_mouseCell = {BINDU::Input::getMousePosition().x - static_cast<int> (m_camera.getCameraMatrix().dx),BINDU::Input::getMousePosition().y - static_cast<int>(m_camera.getCameraMatrix().dy)};

		m_mouseCell.x = (m_mouseCell.x - tileLayer->getOffsetX()) / scene->getWorldTileWidth() ;
		m_mouseCell.y = (m_mouseCell.y - tileLayer->getOffsetY()) / scene->getWorldTileHeight() ;
	}
	if (m_mouseCell.x < 18 && m_mouseCell.y < 35)
	{
		int tile = tileLayer->getMapData()[m_mouseCell.y][m_mouseCell.x];
		//tileLayer->getMapData()[m_mouseCell.y][m_mouseCell.x] = 0;

		BINDU::TileSet* tileSet = nullptr;

		for (auto& m : scene->getTileSets())
		{
			if (tile <= m->lastGid && tile >= m->firstGid)
			{
				tileSet = m.get();
				break;
			}
		}

		if (tileSet)
		{

			tile -= tileSet->firstGid;

			tileLayer->getTile(m_mouseCell.y, m_mouseCell.x)->sprite.setOpacity(tileLayer->getTile(m_mouseCell.y, m_mouseCell.x)->sprite.getOpacity() - 10);
		}
	}
	m_font.Draw(graphics, m_camera.getCameraMatrix());

}

void MainScene::ProcessInput()
{
	m_paddle->ProcessInput();

	if (BINDU::Input::isKeyPressed(BND_V))
		m_camera.setZoom(m_zoomLevel+=0.1f);
	if (BINDU::Input::isKeyPressed(BND_B))
		m_camera.setZoom(m_zoomLevel-=0.1f);
	

}
