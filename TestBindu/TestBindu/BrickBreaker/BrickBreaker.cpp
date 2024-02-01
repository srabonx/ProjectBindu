
#include <Bindu.h>



class LevelManager
{
private:
	BINDU::Sprite m_background;
	BINDU::Sprite m_boundary;

public:

	LevelManager() = default;
	~LevelManager() = default;

	void Init()
	{
		
	}

	void LoadResources()
	{
		m_background.LoadSpriteFromFile(L"Resource/images/background.png");
		m_background.setPosition(0.f, 0.f);
		m_background.setSize(g_engine->getWindowWidth(), g_engine->getWindowHeight());

		m_boundary.LoadSpriteFromFile(L"Resource/images/borders.png");
		m_boundary.setPosition(50, 50);
		m_boundary.setSize(m_background.getSize().width * 70, m_background.getSize().height * 70);

	}

	void Draw(BINDU::Graphics* graphics,const D2D1_MATRIX_3X2_F& cameraMatrix )
	{
		m_background.Draw(graphics, cameraMatrix);
		m_boundary.Draw(graphics, cameraMatrix);
	}

};


class BrickBreaker
{
private:
	LevelManager m_levelManager;

public:

	void Init()
	{
		BINDU::EngineProperties props;
		props.versionNotice = true;
		props.windowWidth = 640;
		props.windowHeight = 600;
		props.windowTitle = "BrickBreaker";

		g_engine->setEngineProperties(props);

		m_levelManager.Init();
		m_levelManager.LoadResources();
	}

	void Draw(BINDU::Graphics* graphics)
	{
		m_levelManager.Draw(graphics, D2D1::Matrix3x2F::Identity());
	}

};