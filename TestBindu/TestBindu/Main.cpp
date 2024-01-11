#include <Bindu.h>
//#include <HelperMethods.h>

#include <vector>
#include "Player.h"

using namespace BINDU;
#define MAX_OBJ 5

bool BINDU::e_gameOver = false;
class MainGame
{
private:
	bool processorMax;
	Player m_player;
	ComPtr<ID2D1Bitmap> bitmap;
	ID2D1Bitmap* bitmap2;
	Sprite* m_sprite;
	std::vector<Sprite*> m_vec;
	ParticleEmitter m_emitter;
	ParticleEmitter emitter2;
	SpriteBatch m_spriteBatch;
	float m_posX{ 0 }, m_posY{ 0 };
public:
	MainGame()
	{
		processorMax = false;
		m_sprite = nullptr;

	}

	~MainGame()
	{

		bitmap2->Release();
 		if (m_vec.size() > 0)
 		{
			std::vector<Sprite*>::iterator itr;
 
			for (itr = m_vec.begin(); itr != m_vec.end(); )
			{
				if ((*itr))
				{
					delete (*itr);
					(*itr) = nullptr;
					itr = m_vec.erase(itr);
				}
				else
					itr++;

			}
			m_vec.clear();
		}
 		
		if (!m_sprite)
			delete m_sprite;
	}

	bool Preload()
	{
//		g_engine->sendMessage(g_engine->getVersionText().c_str(), "Engine Notice!");
//		g_engine->setTitle("Worked");
//		g_engine->setWindowWidth(640);
//		g_engine->setWindowHeight(360);
//		g_engine->setAppIcon("Resource/Bindu.ico");

		EngineProperties props;
		props.versionNotice = true;
		props.windowTitle = "Worked";
		props.windowWidth = 1280;
		props.windowHeight = 800;
		props.windowIcon = "Resource/Bindu.ico";

		g_engine->setEngineProperties(props);

		return true;
	}

	bool Init()
	{
		ParticleProps props{};

		props.size = { 16.f,16.f };
		props.velocity = { 40.f,40.f };
		props.startScale = 0.5f;
		props.endScale = 0.f;
		props.startColor = { 200,220,255,20 };
		props.endColor = { 255,0,0,10 };
		props.colorRandomnessRange = iRange(0, 255);
		props.colorOpacityRange = iRange(0, 255);
		props.minTimetoChangeColor = 0.f;
		props.fadeOut = true;
		props.rotation = 0;
		props.rotationRate = 20;
		props.lifeTime = 5.f;

		m_emitter.Init(props);
		m_emitter.LoadParticleSprite(L"Resource/particle16.png");
		m_emitter.setPosition({ 400.f,400.f });		
		m_emitter.setDirection(0);
		m_emitter.setSpread(360);
		m_emitter.setEmissionInterval(0);
		m_emitter.setEmissionRate(5);
		m_emitter.setMax(1000);
		m_emitter.onLoop(false);

		props.size = { 16.f,16.f };
		props.velocity = { 60.f,60.f };
		props.startScale = 5.f;
		props.endScale = 10.f;
		props.startColor = { 220,250,255,20 };
		props.endColor = { 250,0,0,200 };
		props.colorRandomnessRange = iRange(0, 0);
		props.colorOpacityRange = iRange(10, 20);
		props.minTimetoChangeColor = 0.f;
		props.fadeOut = true;
		props.rotation = 0;
		props.rotationRate = 20;
		props.lifeTime = 6.f;

		emitter2.Init(props);
		emitter2.LoadParticleSprite(L"Resource/smoke.png");
		emitter2.setPosition({ 600.f,400.f });
		emitter2.setDirection(240);
		emitter2.setSpread(60);
		emitter2.setEmissionInterval(20);
		emitter2.setEmissionRate(3);
		emitter2.setMax(1000);
		emitter2.onLoop(true);


		
		BitmapLoader::LoadFromFile(L"Resource/asteroid.png", bitmap.ReleaseAndGetAddressOf());

		for (int i = 0; i < 100; i++) {
			BitmapLoader::LoadFromFile(L"Resource/Bindu.png", &bitmap2);
		}
		
 		for (int i = 1; i <= MAX_OBJ; i++)
		{
			m_sprite = new Sprite();
			m_sprite->SetBitmap(bitmap.Get());
			m_sprite->setPosition(rand()%1280, rand()%800);
			m_sprite->setSize(60, 60);
			m_sprite->setTotalFrame(64);
			m_sprite->setTotalColumn(8);
			m_sprite->setCurrentFrame(rand()%64);
			m_sprite->setRotation(rand()% 7);
			m_sprite->setRotationTimer(rand()%20);
			m_sprite->doesRotate(true);
			m_sprite->setScaleDelta(0.03f);
			m_sprite->setScaleTimer(20);
			m_sprite->setScaleRatio( 1, 1+ rand()% 5);
			m_sprite->doesScale(true);
			m_sprite->setAnimationFrameWidth(60);
			m_sprite->setAnimationFrameHeight(60);
			m_sprite->setAnimationTimer(100);
     		m_sprite->setAnimated(true);
			m_vec.push_back(m_sprite);
		}
		


 		m_player.Init();

		m_spriteBatch.Init();
		m_spriteBatch.LoadSpriteSheet(L"Resource/particle16.png");
		m_spriteBatch.setSpriteCount(5);
		D2D1_RECT_F dstRect{};
		for (int i = 0; i < 1000; i++) {

			float x = rand() % 50;
			float y = rand() % 50;

			dstRect = { x,y,x + 16,y + 16 };
			BND_COLOR color{};
			color.r = RandomNumber::Get(250, 255);
			color.g = RandomNumber::Get(250, 255);
			color.b = RandomNumber::Get(250, 255);
			color.a = RandomNumber::Get(1, 20);


			m_spriteBatch.SetSprite( dstRect, NULL, color, 4, rand() % 20,{(float)(rand()%200),(float)(rand() % 10)});

		}
		return true;
	}

	void Update(float dt)
	{

 		if (Input::isKeyPressed(BND_X))
 			g_engine->setMaximizeProcessor(processorMax = !processorMax);
 

		if (Input::isMouseButtonHold(BND_BTN_LEFT))
		{
			m_emitter.setPosition({(float) Input::getMousePosition().x,(float) Input::getMousePosition().y });
			m_emitter.Generate();
		}

		if (Input::isMouseButtonPressed(BND_BTN_RIGHT))
			emitter2.setPosition({ (float)Input::getMousePosition().x,(float)Input::getMousePosition().y });

		if (Input::isKeyPressed(BND_L))
			m_emitter.onLoop(true);
		if (Input::isKeyPressed(BND_P))
			m_emitter.onLoop(false);

		m_emitter.Update(dt);
		emitter2.Update(dt);
		//m_emitter.setDirection(m_posX++);
 		m_player.Update(dt);

// 		for (auto& m : m_vec)
// 		{
// 			m->Update(dt);
// 		}

//		m_spriteBatch.Update(dt);
		
	}

	void ProcessInput()
	{
		m_player.ProcessInput();
	}

	void Render(Graphics* graphics)
	{
		graphics->getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Black));
 		m_player.Draw(graphics);
//		m_spriteBatch.Draw(graphics);

 //		for (auto& m : m_vec)
 //		{
 //			m->Draw(graphics);
//		}

		m_emitter.Draw(graphics);
		emitter2.Draw(graphics);
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



