#ifndef MENUSCENE_H
#define MENUSCENE_H
#include "Camera.h"
#include "ParticleEmitter.h"
#include "Scene.h"
#include <SoundSystem.h>

#include "Font.h"

class MenuScene:public BINDU::Scene
{
private:

	BINDU::Texture m_buttonTexture;
	BINDU::Texture m_selectorTexture;
	BINDU::Texture m_bgTexture;

	BINDU::Sprite* m_bgImage = nullptr;

	BINDU::Sprite*  m_playButton = nullptr;
	BINDU::Animator	m_playButtonAnimator;

	BINDU::Sprite*  m_optionButton = nullptr;
	BINDU::Animator m_optionButtonAnimator;

	BINDU::Sprite*  m_exitButton = nullptr;
	BINDU::Animator m_exitButtonAnimator;

	bool			m_switchToGame{ false };
	bool			m_switchToOption{ false };
	bool			m_switchToQuit{ false };
	

	BINDU::Sprite*  m_selector = nullptr;

	BINDU::Animator m_selectorAnimator;

	BINDU::Sprite* m_musicOnButton = nullptr;
	BINDU::Sprite* m_musicOffButton = nullptr;
	BINDU::Sprite* m_sfxOnButton = nullptr;
	BINDU::Sprite* m_sfxOffButton = nullptr;
	BINDU::Sprite* m_backButton = nullptr;

	BINDU::Layer* m_fgLayer = nullptr;
	BINDU::Layer* m_optionLayer = nullptr;
	BINDU::Layer* m_bgLayer = nullptr;
	BINDU::Layer* m_starEmitterLayer = nullptr;

	BINDU::ParticleEmitter* m_starEmitter = nullptr;

	BINDU::Camera m_camera;

	BINDU::Font		m_font;
	BINDU::Font		m_movingFont;
	BINDU::Vec2f	m_fontPos;

private:

	void SetParticleEmitter();
	void UpdateMovingFont(float dt);

public:

	void Init();

	void onLoadResource() override;
	void onReleaseResource() override;

	void Update(float dt) override;
	void Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
	void ProcessInput() override;

};

#endif // MENUSCENE_H
