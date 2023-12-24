#include "Bindu.h"
#pragma once


namespace BINDU {

	enum RenderTargetType
	{
		HWND_RENDERTARGET = 1 << 0,			// 00001 == 1
		DXGI_RENDERTARGET = 1 << 1,			// 00010 == 2
		BITMAP_RENDERTARGET = 1 << 2,		// 00100 == 3
	};

	class Graphics {
	private:
		HWND m_windowHandle;														// Handle to window
		ComPtr<ID2D1Factory4> m_d2d1Factory;										// Direct2D 1 Factory 4

		ComPtr<ID2D1Device3> m_d2d1Device;											// Direct2D 1 device 3
		ComPtr<ID2D1DeviceContext3> m_d2d1DeviceContext;							// Direct2D 1 device context 3

		ComPtr<ID2D1HwndRenderTarget> m_d2d1HwndRenderTarget;						// Direct2D 1 hwnd Render Target
		ComPtr<ID2D1RenderTarget> m_d2d1DxgiRenderTarget;							// Direct2D 1 DXGI render target
		ComPtr<ID2D1Bitmap1> m_d2d1BitmapTarget;									// Direct2D 1 bitmap1 to use as render target for m_d2d1DeviceContext

		ComPtr<ID2D1SolidColorBrush> m_d2d1SolidColorBrush;							// Direct2D 1 solid color Brush
		ComPtr<ID2D1LinearGradientBrush> m_d2d1LinGradientBrush;					// Direct2D 1 linear gradient brush

		
		ComPtr<IDXGIFactory2> m_dxgiFactory;										// DXGI Factory 2
		ComPtr<IDXGIDevice2> m_dxgiDevice;											// DXGI Device 2
		ComPtr<IDXGIAdapter> m_dxgiAdapter;											// DXGI adapter
		ComPtr<IDXGISwapChain3> m_dxgiSwapChain;									// DXGI swapChain 1
		ComPtr<IDXGISurface> m_dxgiSurface;											// DXGI surface for rendering

		ComPtr<ID3D11Device3>				m_d3d11Device;							// Direct3D 11 device 
		ComPtr<ID3D11DeviceContext3>		m_d3d11DeviceContext;					// Direct3D 11 device context

		ComPtr<ID3D11RenderTargetView>		m_d3d11RenderTargetView;				// Direct3D 11 render target view
		ComPtr<ID3D11DepthStencilView>		m_d3d11DepthStencilView;				// Direct3D 11 depth stencil view

		ComPtr<ID3D11Debug>					m_d3d11Debug;
		ComPtr<ID3D11Texture2D>				m_d3d11Backbuffer;						// Direct3D 11 Texture 2D to use as backbuffer
		
		int									m_outputWidth;
		int									m_outputHeight;
		DXGI_MODE_ROTATION					m_outputRotation;

		D3D_FEATURE_LEVEL m_d3dFeatureLevel;										// Direct3D feature level
		DXGI_PRESENT_PARAMETERS m_dxgiPP;											// DXGI present parameters

		RenderTargetType m_renderTargetType;										// Render Target type (HWND / DXGI)

	private:
		HRESULT CreateDeviceIndependentResource();									// Creates device independent resources. Such as Factories.

		HRESULT CreateDeviceResource();												// Creates device dependent resources. Such as Render Targets and other resources.
		
		void DiscardDeviceResources();												// (Redundant). Discards device dependent resources if device is lost. 

		HRESULT CreateD2dDevice(ID2D1Device3** ppDevice, const ComPtr<IDXGIDevice>& dxgiDevice);							// Creates Direct2D device

		HRESULT CreateD2dDeviceContext(ID2D1DeviceContext3** ppContext);				// Creates Direct2D device context.(must have D2D device created)

		HRESULT CreateD2dBrushes(ID2D1RenderTarget* renderTarget);					// Creates brushes for particular Render Target
		
		HRESULT CreateD3dDevice(D3D_DRIVER_TYPE driverType,							// Creates Direct3D device
								UINT flags, ID3D11Device** ppDevice,
								ID3D11DeviceContext** ppDeviceContext);

		HRESULT CreateD2d1HwndRenderTarget(const D2D1_SIZE_U& rtSize,				// Creates Direct2D 1 HWND render target.
									 ID2D1HwndRenderTarget** ppHwndRt);										

		HRESULT CreateD2d1DxgiRenderTarget(const D2D1_SIZE_U& rtSize,				// Creates Direct2D 1 DXGI render target.
									 ID2D1RenderTarget** ppHwndRt);			

		HRESULT CreateD2d1BitmapRenderTarget();										// Creates Direct2D 1 Bitmap as render target.

		void CreateD3DDevice();
		void CreateResources();
		void OnDeviceLost();



	public:
		Graphics(const HWND& hWnd);													// Constructor.. Takes in the handle to the window this graphics class will render to.
		void GetDebug(ID3D11Debug** debug);
		~Graphics();																// Destructor. uses the default one.
		
		HRESULT Init();																// Initializes the necessary components
		
		void OnRender();															// (Redundant). Encapsulates the drawing commands in a single function.
		
		HRESULT BeginDraw();														// Begins the drawing sequence for a particular render target. All drawing operations must be after this one.
		
		//void Clear(D2D1::ColorF color = D2D1::ColorF::White);						// Clears the render target with the color specified.
		
		HRESULT EndDraw();															// Ends the drawing sequence.
		
		void resizeRenderTarget(int width, int height);								// Resizes the particular render target (if supports).

		void Clear();
		void Present();

		// Accessor / Mutator functions

		inline void setRenderTargetType(uint16_t type) { m_renderTargetType = RenderTargetType(type); }			// Sets the render target type.

		ID2D1RenderTarget* getRenderTarget();															// Returns the current render target.
		
		ID2D1SolidColorBrush* getSolidColorBrush() { return m_d2d1SolidColorBrush.Get(); }				// Returns the D2d1 solid color brush.
		
		ID2D1LinearGradientBrush* getLinearGradientBrush() { return m_d2d1LinGradientBrush.Get(); }		// Returns the D2d1 linear gradient brush.
		
		IDXGISurface* getDxgiSurface() { return m_dxgiSurface.Get(); }									// Returns the DXGI surface.
		
		ID2D1Factory* getD2DFactory() { return m_d2d1Factory.Get(); }									// Returns the Direct2D1 factory.

		ID2D1DeviceContext3* getD2dDeviceContext() { return m_d2d1DeviceContext.Get(); }					// Returns the Direct2D1 Device context.

	};// class
};// namespace

