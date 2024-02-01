#pragma once
#include <Bindu.h>

#include "Constants.h"
#include "Font.h"
#include "MainScene.h"
#include "SceneManager.h"

class GameManager: public BINDU::SceneManager
{
private:

	BINDU::Bnd_Size		m_worldSize{ TILE_SIZE.x * 25,TILE_SIZE.y * 45 };

	std::unique_ptr<MainScene>		m_mainScene;

	BINDU::Font			m_font;

public:
	GameManager() = default;
	~GameManager() = default;

	void Preload();

	void Init();

	void onLoadResource();

	void Update(float dt);

	void Draw(BINDU::Graphics* graphics);

};

