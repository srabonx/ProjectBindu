#pragma once
#include <Bindu.h>

#include "Camera.h"
#include "Constants.h"
#include "Layer.h"
#include "MapParser.h"
#include "Paddle.h"
#include "Sprite.h"
#include <Font.h>

class MainScene
{
private:

	BINDU::Texture						m_bgTexture;
	BINDU::Texture						m_boundaryTexture;

	std::unique_ptr<BINDU::Layer>		m_bgLayer;
	std::unique_ptr<BINDU::Layer>		m_fgLayer;

	std::unique_ptr<BINDU::Sprite>		m_backgroundImg;
	std::unique_ptr<BINDU::Sprite>		m_boundaryImg;

	int									m_boundaryImgOffset{ 8 };

	std::unique_ptr<Paddle>				m_paddle;

	std::string							m_levelData;

	std::vector<std::vector<int>>		m_mapData;
	BINDU::MapParser					m_mapParser;
	std::unique_ptr<BINDU::Scene>		scene;
	BINDU::TileLayer*					tileLayer = nullptr;

	BINDU::Vec2i						m_mouseCell{};

	BINDU::Font			m_font;


	BINDU::Camera		m_camera;
	float				m_zoomLevel{};

public:

	MainScene() = default;
	~MainScene() = default;

	void	Init();

	void    onLoadResource(const BINDU::Bnd_Size& windSize);

	void    Update(float dt);

	void	Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix);

	void    ProcessInput();



};

