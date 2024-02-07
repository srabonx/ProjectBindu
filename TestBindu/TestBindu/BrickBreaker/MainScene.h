#pragma once
#include <Bindu.h>

#include "Camera.h"
#include "Constants.h"
#include "Layer.h"
#include "MapParser.h"
#include "Paddle.h"
#include "Sprite.h"
#include <Font.h>

#include "ParticleEmitter.h"


class MainScene:public BINDU::Scene
{
private:

	BINDU::Bnd_Circle					m_circle;
	BINDU::Vec2f						circleVel;


	BINDU::Layer*						m_bgLayer1 = nullptr;
	BINDU::Layer*						m_bgLayer2 = nullptr;
	BINDU::Layer*						m_fgLayer = nullptr;

	BINDU::Sprite*						m_backgroundImg = nullptr;
	BINDU::Sprite*						m_boundaryImg = nullptr;

	BINDU::ParticleEmitter*				m_particleEmitter = nullptr;

	int									m_boundaryImgOffset{ 8 };

	Paddle*								m_paddle  = nullptr;

	BINDU::TileLayer*					m_tileLayer = nullptr;

	BINDU::Vec2f						m_mouseCell{};

	BINDU::Font							m_font;

	BINDU::Camera						m_camera;

	BINDU::MapParser					m_mapParser;

	float								m_zoomLevel{1};

	int									m_currentLevel{};
	int									m_totalLevel{};

	bool								m_switchLevel{ false };
	bool								m_gameOver{ false };
	bool								m_pauseMode{ false };
	bool								m_transitionPhase{ false };

	int									m_score{ 0 };
	int									m_highScore{ 0 };

private:

	void	ResetMap();
	void	GameComplete();
	void    SwitchLevel();
	void    TransitionNext();
	void    Pause();
	void	SetParticleEmitter();

public:

	MainScene() = default;
	~MainScene() = default;

	void	Init();

	void    onLoadResource(const BINDU::Bnd_Size& windSize);

	void    Update(float dt) override;

	void	Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;

	void    ProcessInput() override;



};

