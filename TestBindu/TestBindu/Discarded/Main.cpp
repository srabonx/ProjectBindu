#include <Bindu.h>
//#include <HelperMethods.h>
#include <dwrite.h>
#include <vector>
#include "Player.h"
#include <initConsole.h>

using namespace BINDU;
#define MAX_OBJ 5

bool BINDU::e_gameOver = false;
class MainGame
{
private:
	bool processorMax;
	std::unique_ptr<Player> m_player;
	std::unique_ptr<Sprite> sprite2;
	std::unique_ptr< ParticleEmitter> emitter2;

	ComPtr<ID2D1Bitmap> bitmap;
	ID2D1Bitmap* bitmap2;
	std::unique_ptr<Sprite> m_sprite;
	std::vector<Sprite*> m_vec;
	ParticleEmitter m_emitter;
	
	SpriteBatch m_spriteBatch;
	float m_posX{ 0 }, m_posY{ 0 };

	SceneManager m_sceneManager;
	std::unique_ptr<Layer> backgroundLayer;
	std::unique_ptr<Layer> foregroundLayer;
	std::unique_ptr<Scene> m_scene;

	Font m_font;

	Camera m_camera;


public:
	MainGame()
	{
		processorMax = false;
		m_sprite = nullptr;
	}

	~MainGame()
	{

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
		props.windowWidth = 800;
		props.windowHeight = 600;
		props.windowIcon = "Resource/Bindu.ico";

		g_engine->setEngineProperties(props);

		return true;
	}

	bool Init()
	{
		InitConsole();
		ParticleProps props{};

		props.size = { 16.f,16.f };
		props.velocity = { 40.f,40.f };
		props.startScale = 0.5f;
		props.endScale = 2.f;
		props.startColor = { 255,215,0,20 };
		props.endColor = { 255,0,0,10 };
		props.colorRandomnessRange = iRange(0, 255);
		props.colorOpacityRange = iRange(0, 255);
		props.minTimetoChangeColor = 0.f;
		props.fadeOut = true;
		props.rotation = 0;
		props.rotationRate = 20;
		props.lifeTime = 16.f;

		m_emitter.Init(props);
		m_emitter.LoadParticleSprite(L"Resource/particle16.png");
		m_emitter.setPosition({ 400.f,400.f });
		m_emitter.setDirection(0);
		m_emitter.setSpread(360);
		m_emitter.setEmissionInterval(0);
		m_emitter.setEmissionRate(70);
		m_emitter.setMax(20000);
		m_emitter.onLoop(false);

		props.size = { 16.f,16.f };
		props.velocity = { 60.f,60.f };
		props.startScale = 5.f;
		props.endScale = 10.f;
		props.startColor = { 255,215,0,20 };
		props.endColor = { 250,0,0,200 };
		props.colorRandomnessRange = iRange(245, 255);
		props.colorOpacityRange = iRange(10, 20);
		props.minTimetoChangeColor = 0.f;
		props.fadeOut = true;
		props.rotation = 0;
		props.rotationRate = 20;
		props.lifeTime = 14.f;

		emitter2 = std::make_unique<ParticleEmitter>();
		emitter2->Init(props);
		emitter2->LoadParticleSprite(L"Resource/smoke.png");
		emitter2->setPosition({ 600.f,600.f });
		emitter2->setSize({ 50.f,50.f });
		emitter2->setDirection(240);
		emitter2->setSpread(70);
		emitter2->setEmissionInterval(20);
		emitter2->setEmissionRate(6);
		emitter2->setMax(3000);
		emitter2->onLoop(true);

		emitter2->setActive(true);
		emitter2->setColliderOffset(0, 0, 0, 0);
		emitter2->setCollideAble(true);

		m_sprite = std::make_unique<Sprite>();

		m_sprite->LoadSpriteFromFile(L"Resource/ointyArt.png");

		m_sprite->setPosition(0, 0);
		m_sprite->setSize(1280, 800);
		m_sprite->setActive(true);




		m_player = std::make_unique<Player>();
		sprite2 = std::make_unique<Sprite>();
		m_scene = std::make_unique<Scene>();

		backgroundLayer = std::make_unique<Layer>();
		foregroundLayer = std::make_unique<Layer>();

		m_player->Init();
		m_player->setPosition(200, 100);
		m_player->setRotation(0);
		m_player->setActive(true);
		m_player->setColliderOffset(40, 20, 100, 35);
		m_player->setCollideAble(true);

		sprite2->LoadSpriteFromFile(L"Resource/Bindu.png");

		sprite2->setPosition(50, 50);
		sprite2->setSize(60, 60);
		sprite2->setActive(true);
		
		sprite2->setCollidable(true);

		m_sprite->setCollidable(false);



		backgroundLayer->AddObject(std::move(m_sprite), "bgimage");
		backgroundLayer->setActive(true);
		//foregroundLayer->AddObject(std::move(m_sprite), "bdimage");
		foregroundLayer->AddObject(std::move(m_player), "mainPlayer");
		foregroundLayer->AddObject(std::move(sprite2), "sprite2");
		foregroundLayer->AddObject(std::move(emitter2), "emitter2");
		foregroundLayer->setActive(true);
		foregroundLayer->showDebug(true);
		backgroundLayer->setParallaxFactor({ 0.5f, 0.5f });

		m_scene->AddLayer(std::move(backgroundLayer), "backgroundLayer");
		m_scene->AddLayer(std::move(foregroundLayer), "foregroundLayer");
		m_scene->setActive(true);

		m_sceneManager.AddScene(std::move(m_scene), "firstScene");

		m_camera.SetTarget(*m_sceneManager.getScene("firstScene")->getLayer("foregroundLayer")->getObject("mainPlayer"));
		
		m_font.Init();

		m_font.LoadBitmapFont(L"Resource/unispace-bitmapfont.png");
		m_font.setCharSize(9, 20);

		
		return true;
	}

	void Update(float dt)
	{

		if (Input::isKeyPressed(BND_X))
			g_engine->setMaximizeProcessor(processorMax = !processorMax);


		if (Input::isMouseButtonHold(BND_BTN_LEFT))
		{
			m_emitter.setPosition({ (float)Input::getMousePosition().x - m_camera.getCameraMatrix().dx,(float)Input::getMousePosition().y - m_camera.getCameraMatrix().dy });
			m_emitter.Generate();
		}

		if (Input::isMouseButtonHold(BND_BTN_RIGHT))
		{
			//	emitter2.setPosition({ (float)Input::getMousePosition().x,(float)Input::getMousePosition().y });

			Vec2f mousePos = { static_cast<float>(Input::getMousePosition().x - m_camera.getCameraMatrix().dx),static_cast<float>(Input::getMousePosition().y - m_camera.getCameraMatrix().dy) };
			Vec2f emitterPos = m_sceneManager.getScene("firstScene")->getLayer("foregroundLayer")->getObject("emitter2")->getPosition();

			float angle = mousePos.Angle(emitterPos);

			ParticleEmitter* temp = dynamic_cast<ParticleEmitter*> (m_sceneManager.getScene("firstScene")->getLayer("foregroundLayer")->getObject("emitter2"));
			temp->setDirection(angle);
			std::cout << angle << std::endl;
		}

		if (Input::isKeyPressed(BND_T))
			m_sceneManager.getScene("firstScene")->getLayer("backgroundLayer")->AddObject(m_sceneManager.getScene("firstScene")->getLayer("foregroundLayer")->RemoveObject("emitter2"),"emitter2");
		if (Input::isKeyHold(BND_O))
			m_sceneManager.getScene("firstScene")->getLayer("foregroundLayer")->getObject("mainPlayer")->setRotation(m_posX++);

		if (Input::isKeyPressed(BND_L))
			m_emitter.onLoop(true);
		if (Input::isKeyPressed(BND_P))
			m_emitter.onLoop(false);

		m_emitter.Update(dt);

		m_camera.Update(dt,* m_sceneManager.getScene("firstScene")->getLayer("foregroundLayer")->getObject("mainPlayer"));
		m_sceneManager.Update(dt);

		
	}

	void ProcessInput()
	{
		m_sceneManager.ProcessInput();
		
	}

	void Render(Graphics* graphics)
	{
		graphics->getRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Black));


		m_sceneManager.Draw(graphics,m_camera.getCameraMatrix());
//		m_spriteBatch.Draw(graphics);

 //		for (auto& m : m_vec)
 //		{
 //			m->Draw(graphics);
//		}

		m_emitter.Draw(graphics,m_camera.getCameraMatrix());
	//	emitter2.Draw(graphics,m_camera.getCameraMatrix());


		m_font.PrintText(500, 500, std::to_string(m_camera.getCameraMatrix().dx), { 255,255,255,255 }, 1);
		m_font.PrintText(20, 10, "Real: " + std::to_string(g_engine->getRealFrameRate()), { 255,255,255,255 },1);
		m_font.PrintText(20, 30, "Core: " + std::to_string(g_engine->getCoreFrameRate()), { 255,255,255,255 }, 1);

		m_font.PrintText(500, 300, "ABCDabcd", {255,255,255,255}, 1);


		//m_font.PrintText(500, 400, std::to_string(tempPlayer->getPosition().x), { 255,255,255,255 },1);

	}

	void checkCollision(SceneObject* a,SceneObject* b)
	{

		if(Player* pA = dynamic_cast<Player*>(a))
		{
			if (Sprite* pB = dynamic_cast<Sprite*>(b))
			{
				pB->setPosition(pA->getPosition().x + 144, pB->getPosition().y);
			}
			if(ParticleEmitter * pB = dynamic_cast<ParticleEmitter*> (b))
			{
				pB->setPosition(pA->getPosition().x + 144, pB->getPosition().y);
			}
			
		}		

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

	void BINDU::game_collision(SceneObject* a, SceneObject* b)
	{
		game->checkCollision(a, b);
	}
 



