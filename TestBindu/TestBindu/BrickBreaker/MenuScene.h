#ifndef MENUSCENE_H
#define MENUSCENE_H
#include "Camera.h"
#include "Scene.h"

class MenuScene:public BINDU::Scene
{
private:

	BINDU::Texture m_buttonTexture;
	BINDU::Texture m_selectorTexture;
	BINDU::Texture m_bgTexture;

	BINDU::Sprite* m_bgImage = nullptr;

	BINDU::Sprite*  m_playButton = nullptr;
	BINDU::Sprite*  m_optionButton = nullptr;
	BINDU::Sprite*  m_exitButton = nullptr;

	BINDU::Sprite*  m_selector = nullptr;

	BINDU::Animator m_animator;

	BINDU::Layer* m_fgLayer = nullptr;
	BINDU::Layer* m_bgLayer = nullptr;

	BINDU::Camera m_camera;

public:

	void Init();

	void onLoadResource() override;

	void Update(float dt) override;
	void Draw(BINDU::Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
	void ProcessInput() override;

};

#endif // MENUSCENE_H
