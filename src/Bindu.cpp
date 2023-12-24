#include "Include/Bindu.h"

namespace BINDU {
	//Engine* Engine::s_Instance = nullptr;
	
 Engine::Engine()
 {
	 m_hWnd = NULL;
	 m_versionMajor = VERSION_MAJOR;
	 m_versionMinor = VERSION_MINOR;
	 m_revision = REVISION;
	 m_titleName = NULL;
	 m_appIcon = NULL;
	 m_width = 0;
	 m_height = 0;
	 m_doneInit = false;
	 m_frameCountCore = 0;
	 m_frameCountReal = 0;
	 m_frameRateCore = 0;
	 m_frameRateReal = 0;
	 m_deltaTime = 0;
	 m_maximizeProcessor = false;
	 m_lastFrameTime = GetTickCount();
	 errorType = N_ERR;
	 m_isDown = false;
	 m_wasDown = false;
	 m_VKcode = 0;

	 // Setting object pointers to NULL
	 o_graphics = NULL;
 }
 Engine::~Engine() 
 {
	 delete o_graphics;
	// m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
 }

 bool Engine::Init()
 {
	 
	 // Initialize Graphics
	 o_graphics = new Graphics(m_hWnd);
	 o_graphics->setRenderTargetType(BITMAP_RENDERTARGET);
	 o_graphics->Init();
	// o_graphics->GetDebug(m_debug.ReleaseAndGetAddressOf());
	 m_doneInit = true;
	// m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	 return true;
 }

 bool Engine::Run()
 {
	 // Count core framerate
	 m_frameCountCore++;
	 if (o_coreTimer.stopwatch(999))
	 {
		 m_frameRateCore = m_frameCountCore;
		 m_frameCountCore = 0;
	 }

	 SetWindowText(m_hWnd, ("FrameRate core: " + std::to_string(m_frameRateCore) + " , FrameRate real: " + std::to_string(m_frameRateReal)).c_str());


	 if (!o_updateTimer.stopwatch(14))
	 {
		 if (!this->isMaximizeProcessor()) {
			 Sleep(1);
		 }
	 }
	 else {
		 m_deltaTime = (GetTickCount() - m_lastFrameTime) / 1000.0f;		// Delta time
		 m_lastFrameTime = GetTickCount();

		 // Count real framerate
		 m_frameCountReal++;
		 if (o_realTimer.stopwatch(999))
		 {
			 m_frameRateReal = m_frameCountReal;
			 m_frameCountReal = 0;

		 }
		 // Process inputs
		 {
			 game_processInputs();
		 }

		 // Update game logic
		 {
			 game_update(m_deltaTime);
		 }

		 // Drawing/ Rendering
		 {
			 o_graphics->Clear();
			 //o_graphics->Clear(D2D1::ColorF::Red);

			// game_render2d(o_graphics);
		
			 o_graphics->Present();
		 }

	 }
	 return true;
 }

 void Engine::Shutdown()
 {
	 e_gameOver = true;
 }

 void Engine::resizeWindow(int width, int height)
 {
	 m_width = width;
	 m_height = height;
	 
	 o_graphics->resizeRenderTarget(width, height);

 }

 std::string Engine::getVersionText() const
 {
	 std::ostringstream s;
	 s << "Bindu - " << getVersionMajor() << "." << getVersionMinor() << "." << getRevision();

	 return s.str();
 }

 void Engine::sendMessage(const char* message, const char* title,const e_Error& errorType)
 {
	 MessageBox(m_hWnd, message, title, MB_OK);
	 switch (errorType)
	 {
	 case FATAL_ERROR:
		 Shutdown();
		 break;
	 case MINOR_ERROR:
		 break;
	 default:
		 break;
	 }
 }

 LRESULT Engine::MessageLoop(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam)
 {
	 if (m_doneInit)
	 {
		 switch (msg)
		 {
		 case WM_QUIT:
		 case WM_DESTROY:
		 case WM_CLOSE:
			 e_gameOver = true;
			 PostQuitMessage(0);
			 break;
		 case WM_SIZE:
			 resizeWindow(LOWORD(lParam), HIWORD(lParam));
			 break;
		 case WM_PAINT:
			 Run();
			 break;

		 case WM_KEYDOWN:
		 case WM_SYSKEYDOWN:
		 case WM_SYSKEYUP:
		 case WM_KEYUP:
			 m_VKcode = static_cast<uint32_t>(wParam);
			// WORD keyFlags = HIWORD(lParam);
			 m_wasDown = ((HIWORD(lParam) & 0x4000) == 0x4000);
			 m_isDown = ((HIWORD(lParam)& 0x8000) != 0x8000);
			 Input::processKeyboardInput(m_VKcode, m_wasDown, m_isDown);
			 
			 break;

		 case WM_LBUTTONDOWN:
		 case WM_LBUTTONUP:
		 case WM_RBUTTONDOWN:
		 case WM_RBUTTONUP:
		 case WM_MBUTTONDOWN:
		 case WM_MBUTTONUP:
		 case WM_XBUTTONDOWN:
		 case WM_XBUTTONUP:

			 Input::processMouseInput(wParam, lParam);
			 break;
			 
		 case WM_MOUSEMOVE:
			 Input::updateMousePosition(lParam);
			 break;

		 default:
			 break;
		 }
		

	 }
	 return DefWindowProc(hWnd, msg, wParam, lParam);
 }
}
