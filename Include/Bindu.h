//								 BINDU - A Game Engine Library
// Ahmed Srabon
//- I don't know how far this would go but I'm willing to take a chance write it. Let's find out how far it goes....
//  [12/12/2023 Ahmed Srabon]

#ifndef _BINDU_H
#define _BINDU_H


#include <string>
#include <windows.h>
#include <DirectXColors.h>
#include <wrl.h>

#include "Graphics.h"
#include "SceneObject.h"

using namespace Microsoft::WRL;

#include "TImer.h"




#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define REVISION 2


namespace BINDU {

	enum e_Error {
		N_ERR,
		FATAL_ERROR,
		MINOR_ERROR,
		
	};
	
	extern bool e_gameOver;
	extern bool game_preload();
	extern bool game_init();
	extern void game_update(float dt);
	extern void game_end();
	extern void game_render2d(Graphics* graphics);
	extern void game_processInputs();
	extern void game_collision(SceneObject* a, SceneObject* b);

	struct EngineProperties
	{
		EngineProperties() :versionNotice(true), windowTitle("Default Window"), windowIcon(""), windowWidth(640), windowHeight(320) {}

		bool				versionNotice;
		const char*			windowTitle;
		const char*			windowIcon;
		int 				windowWidth;
		int					windowHeight;
	};

	struct EnginePropertiesEx
	{
		bool				versionNotice;
		const char*		    windowTitle;
		const char*			windowIcon;
		int 				windowWidth;
		int					windowHeight;
		int					targetFps;
		bool				capFps;
		bool				maximizeProcessor;
		uint16_t			renderTargetType;
	};

	class Engine {
	private:
		HWND						m_hWnd;
		int							m_versionMajor;
		int							m_versionMinor;
		int							m_revision;
		e_Error						errorType;
		bool						m_doneInit;
		int							m_frameCountCore;
		int							m_frameRateCore;
		int							m_frameCountReal;
		int							m_frameRateReal;
		float						m_deltaTime;
		DWORD						m_lastFrameTime;

		float						m_updateTime{};
		float						m_renderTime{};
		float						m_updateTimeCount{};
		float						m_renderTimeCount{};

		// Input variables
		bool						m_isDown;
		bool						m_wasDown;
		uint32_t					m_VKcode;

		EnginePropertiesEx			m_enginePropertiesEx;

		float						oneOverThousand{};

	private:			// Core objects
		Graphics*					o_graphics;

		Timer						o_coreTimer;
		Timer						o_realTimer;
		Timer						o_updateTimer;

	private:
		
		//static Engine* s_Instance;

	public:
		Engine();
		
		~Engine();
		
		bool		Init();
		
		bool		Run();
		
		void		Shutdown();
		
		LRESULT		MessageLoop(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam);

		
	public:			// Accessor Mutator functions. (Setter/Getter)
		// Get the core Engine instance
		//inline static Engine* GetInstance() { return s_Instance = (s_Instance != NULL) ? s_Instance : new Engine(); }
		// Get Graphics instance
		inline Graphics*		getGraphics() { return o_graphics; }

		inline void				setWindowHandle(HWND& hWnd) { m_hWnd = hWnd; }

		void					setEngineProperties(const EngineProperties& engineProperties);

		void					setEnginePropertiesEx(const EnginePropertiesEx& enginePropertiesEx);

		inline HWND&			getWindowHandle() { return m_hWnd; }

		void					resizeWindow(int width,int height);

		inline int				getVersionMajor() const{ return m_versionMajor; }

		inline int				getVersionMinor() const{ return m_versionMinor; }

		inline int				getRevision() const { return m_revision; }

		std::string				getVersionText() const;

		inline const char*		getTitle() const { return m_enginePropertiesEx.windowTitle; }

		inline void				setTitle(const char* title) { m_enginePropertiesEx.windowTitle = title; }

		inline int				getWindowWidth() const { return m_enginePropertiesEx.windowWidth; }

		inline void				setWindowWidth(int width) { m_enginePropertiesEx.windowWidth = width; }

		inline int				getWindowHeight() const { return m_enginePropertiesEx.windowHeight; }

		inline void				setWindowHeight(int height) { m_enginePropertiesEx.windowHeight = height; }

		inline void				setAppIcon(const char* icon) { m_enginePropertiesEx.windowIcon = icon; }

		inline const char*		getAppIcon() const { return m_enginePropertiesEx.windowIcon; }

		inline bool				isDoneInit() { return m_doneInit; }

		inline void				setDoneInit(bool value) { m_doneInit = value; }

		void					sendMessage(const char* message,const char* title,const e_Error& errorType = N_ERR);

		inline int				getCoreFrameRate() const { return m_frameRateCore; }

		inline int				getRealFrameRate() const { return m_frameRateReal; }

		inline float			getUpdateTime() const { return m_updateTime; }			// get update time in ms.

		inline float			getRenderTime() const { return m_renderTime; }			// get render time in ms.

		inline void				setMaximizeProcessor(bool value) { m_enginePropertiesEx.maximizeProcessor = value; }

		inline bool				isMaximizeProcessor() const { return m_enginePropertiesEx.maximizeProcessor; }


	}; // class
}// namespace

	extern BINDU::Engine* g_engine;		// Global Engine instance.




#endif
