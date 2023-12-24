#include <Bindu.h>
//#include <HelperMethods.h>

#include <vector>
#include "Player.h"

using namespace BINDU;
#define MAX_OBJ 500

bool BINDU::e_gameOver = false;
class MainGame
{
private:
	bool processorMax;
	Player m_player;
	ComPtr<ID2D1Bitmap> bitmap;
	Sprite* m_sprite;
	std::vector<Sprite*> m_vec;
public:
	MainGame()
	{
		processorMax = false;
		m_sprite = nullptr;
	}

	~MainGame()
	{
		
// 		if (m_vec.size() > 0)
// 		{
// 			std::vector<Sprite*>::iterator itr;
// 
// 			for (itr = m_vec.begin(); itr != m_vec.end(); )
// 			{
// 				if ((*itr))
// 				{
// 					delete (*itr);
// 					(*itr) = nullptr;
// 					itr = m_vec.erase(itr);
// 				}
// 				else
// 					itr++;
// 
// 			}
// 			m_vec.clear();
// 		}
// 		
// 		if (!m_sprite)
// 			delete m_sprite;
	}

	bool Preload()
	{
		g_engine->sendMessage(g_engine->getVersionText().c_str(), "Engine Notice!");
		g_engine->setTitle("Worked");
		g_engine->setWindowWidth(640);
		g_engine->setWindowHeight(360);
		g_engine->setAppIcon("Resource/Bindu.ico");
		

		return true;
	}

	bool Init()
	{
// 		BitmapLoader::LoadFromFile(L"Resource/asteroid.png", &bitmap);
// 
// 		for (int i = 0; i < MAX_OBJ; i++)
// 		{
// 			m_sprite = new Sprite();
// 			m_sprite->SetBitmap(bitmap.Get());
// 			m_sprite->setPosition(rand()%640, rand()%360);
// 			m_sprite->setSize(60, 60);
// 			m_sprite->setTotalFrame(64);
// 			m_sprite->setTotalColumn(8);
// 			m_sprite->setAnimationFrameWidth(60);
// 			m_sprite->setAnimationFrameHeight(60);
// 			m_sprite->setAnimationTimer(100);
// 			m_sprite->setAnimated(true);
// 			m_vec.push_back(m_sprite);
// 		}
// 
// 
// 		m_player.Init();
		
		return true;
	}

	void Update(float dt)
	{

// 		if (Input::isKeyPressed(BND_X))
// 			g_engine->setMaximizeProcessor(processorMax = !processorMax);
// 
// 		m_player.Animate();
// 		m_player.Update(dt);
// 
// 		for (auto& m : m_vec)
// 		{
// 			m->Update(dt);
// 		}
		
	}

	void ProcessInput()
	{
		m_player.ProcessInput();
	}

	void Render(Graphics* graphics)
	{
		graphics->getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
// 		m_player.Draw(graphics);
// 	
// 		for (auto& m : m_vec)
// 		{
// 			m->Draw(graphics);
// 		}

	}
};


	MainGame* game;

	bool BINDU::game_preload()
	{
		game = new MainGame();

		return game->Preload();
	}

	bool BINDU::game_init()
	{
		game->Init();
		return true;
	}

	void BINDU::game_end()
	{
		delete game;
	}

	void BINDU::game_update(float dt)
	{
		game->Update(dt);
	}

	void BINDU::game_render2d(Graphics* graphics)
	{
		game->Render(graphics);
	}

	void BINDU::game_processInputs()
	{
		game->ProcessInput();
	}



