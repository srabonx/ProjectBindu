#pragma once
#include <Bindu.h>

#include "Constants.h"
#include "Font.h"
#include "MainScene.h"
#include "MenuScene.h"
#include "SceneManager.h"
#include "SoundSystem.h"


enum class GameState
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

	static bool			m_musicOn;
	static bool			m_sfxOn;

	static GameState m_state;

	static BINDU::SoundSystem* m_soundSystem;

public:
	GameManager() = default;
	~GameManager();

	void Preload();

	void Init();

	void onLoadResource();

	void Update(float dt);

	void Draw(BINDU::Graphics* graphics);

	static void SetState(GameState state) { m_state = state; }

	static BINDU::SoundSystem* getSoundSystem() { return m_soundSystem; }

	static void setMusic(bool value) { m_musicOn = value; }
	static bool getMusic() { return m_musicOn; }

	static void setSfx(bool value) { m_sfxOn = value; }
	static bool getSfx() { return m_sfxOn; }

};

