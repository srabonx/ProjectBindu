#pragma once
#include <Bindu.h>

#include "Constants.h"
#include "Font.h"
#include "MainScene.h"
#include "MenuScene.h"
#include "SceneManager.h"


enum class State
{
	MENU,
	GAME
};

class GameManager
{
private:

	BINDU::Bnd_Size		m_worldSize{ TILE_SIZE.x * 25,TILE_SIZE.y * 45 };

	std::unique_ptr<MenuScene>		m_menuScene;

	std::unique_ptr<MainScene>		m_mainScene;

	BINDU::SceneManager		m_sceneManager;

	static State m_state;

public:
	GameManager() = default;
	~GameManager() = default;

	void Preload();

	void Init();

	void onLoadResource();

	void Update(float dt);

	void Draw(BINDU::Graphics* graphics);

	static void SetState(State state) { m_state = state; }
};

