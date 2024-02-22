#pragma once
#include <Bindu.h>

#include "Camera.h"
#include "Constants.h"
#include "Layer.h"
#include "MapParser.h"
#include "Paddle.h"
#include "Ball.h"
#include "Sprite.h"
#include <Font.h>

#include "ParticleEmitter.h"
#include "SoundSystem.h"


class MainScene:public BINDU::Scene
{
private:

	enum class State
	{
		TRANSITION,
		READY_PHASE,
		PLAY,
		PAUSE,
	};

	BINDU::Bnd_Size					    m_windSize;

	BINDU::Texture						m_shadowTexture;

	BINDU::Layer*						m_bgLayer1 = nullptr;
	BINDU::Layer*						m_bgLayer2 = nullptr;
	BINDU::Layer*						m_shadowLayer = nullptr;
	BINDU::Layer*						m_fgLayer = nullptr;
	BINDU::Layer*						m_transparentLayer = nullptr;
	BINDU::Layer*						m_fontLayer = nullptr;

	BINDU::Layer*						m_pauseLayer = nullptr;

	BINDU::Sprite*						m_resumeButton = nullptr;
	BINDU::Sprite*						m_menuButton = nullptr;
	BINDU::Sprite*						m_retryButton = nullptr;

	BINDU::Sprite*						m_musicButton = nullptr;
	BINDU::Sprite*						m_sfxButton = nullptr;

	BINDU::Animator						m_buttonAnimator;

	BINDU::Sprite*						m_selector = nullptr;
	BINDU::Animator						m_selectorAnimator;

	bool								m_switchToPlay{ false };
	bool								m_switchToMenu{ false };



	BINDU::Sprite*						m_backgroundImg = nullptr;
	BINDU::Sprite*						m_boundaryImg = nullptr;

	BINDU::ParticleEmitter*				m_particleEmitter = nullptr;

	int									m_boundaryImgOffset{ 8 };

	Paddle*								m_paddle  = nullptr;

	Ball*								m_currentBall = nullptr;

	std::vector<Ball*>					m_balls;

	int									m_activeBalls{};

	int									m_maximumBalls{ 3 };

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
	bool								m_ballDropped{ false };
	bool								m_readyPhase{ false };
	bool								m_pauseMode{ false };
	bool								m_transitionPhase{ false };

	State								m_currentState = State::TRANSITION;
	State								m_prevState;

	int									m_readyPhaseTimer{ 180 };

	int									m_score{ 0 };
	int									m_highScore{ 0 };
	int									m_currentBricks{};

	bool								m_launchBall{ false };

	BINDU::Texture						m_healthTex;
	std::vector<std::unique_ptr<BINDU::Sprite>> m_healths;
	int									m_totalHealth{ 5 };


	BINDU::Texture						m_powerUpTex;
	BINDU::Animator						m_powerUpAnimator;
	std::vector<std::unique_ptr<BINDU::Sprite>>			m_powerUps;

private:

	void	ResetGame();
	void	GameComplete();			//TODO:
	void    SwitchLevel();
	void	GameOver();
	void    TransitionPhase();
	void    UpdateReadyPhase();
	void	SetParticleEmitter();
	void    UpdatePowerUps(const float dt);
	void	LaunchBall(Ball* ball);
	void    UpdateBullet(const float dt);
	void    UpdateBrick(std::unique_ptr<BINDU::Tile>& tile, const float dt);
	void	DropBrick(BINDU::Tile* tile, const float dt);
	void	SetUpHealth();
	void	DecreaseHealth() const;
	void	IncreaseHealth();
	void	UpdateHealth();
	void	SetUpPauseLayer();
	void	UpdatePauseLayer();

	void	ResetShadows() const;

public:

	MainScene() = default;
	~MainScene() = default;

	void	Init(const BINDU::Bnd_Size& windSize);

	void    onLoadResource() override;
	void    onReleaseResource() override;

	void    Update(float dt) override;

	void	Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;

	void    ProcessInput() override;



};

