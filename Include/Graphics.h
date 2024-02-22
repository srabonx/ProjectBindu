#pragma once
#include <cstdint>
#include <d2d1_3.h>
#include <d3d11_3.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace BINDU {

	struct BND_COLOR
	{
		BND_COLOR() :r{ 0.0f }, g{ 0.0f }, b{ 0.0f }, a{ 0.0f }
		{}
		BND_COLOR(const BND_COLOR& color):r{color.r},g{color.g},b{color.b},a{color.a}
		{}
		BND_COLOR(float r,float g,float b, float a):r{r},g{g},b{b},a{a}
		{}

		float r;
		float g;
		float b;
		float a;
	};

	enum RenderTargetType
	{
		HWND_RENDERTARGET = 1 << 0,			// 00001 == 1
		DXGI_RENDERTARGET = 1 << 1,			// 00010 == 2
		BITMAP_RENDERTARGET = 1 << 2,		// 00100 == 3
	};


	class Graphics {
	private:
		HWND m_windowHandle;														// Handle to window
		ComPtr<ID2D1Factory4>				m_d2d1Factory;							// Direct2D 1 Factory 4

		ComPtr<ID2D1Device3>				m_d2d1Device;							// Direct2D 1 device 3
		ComPtr<ID2D1DeviceContext6>			m_d2d1DeviceContext;					// Direct2D 1 device context 3

		ComPtr<ID2D1HwndRenderTarget>		m_d2d1HwndRenderTarget;					// Direct2D 1 hwnd Render Target
		ComPtr<ID2D1RenderTarget>			m_d2d1DxgiRenderTarget;					// Direct2D 1 DXGI render target
		ComPtr<ID2D1Bitmap1>				m_d2d1BitmapTarget;						// Direct2D 1 bitmap1 to use as render target for m_d2d1DeviceContext

		ComPtr<ID2D1SolidColorBrush>		m_d2d1SolidColorBrush;					// Direct2D 1 solid color Brush
		ComPtr<ID2D1LinearGradientBrush>	m_d2d1LinGradientBrush;					// Direct2D 1 linear gradient brush

		ComPtr<IDXGISwapChain3>				m_dxgiSwapChain;						// DXGI swapChain 3

		ComPtr<ID3D11Device3>				m_d3d11Device;							// Direct3D 11 device 3 
		ComPtr<ID3D11DeviceContext3>		m_d3d11DeviceContext;					// Direct3D 11 device context 3

		ComPtr<ID3D11RenderTargetView>		m_d3d11RenderTargetView;				// Direct3D 11 render target view
		ComPtr<ID3D11DepthStencilView>		m_d3d11DepthStencilView;				// Direct3D 11 depth stencil view

		ComPtr<ID3D11Debug>					m_d3d11Debug;							// Direct3D debug layer
		ComPtr<ID3D11Texture2D>				m_d3d11Backbuffer;						// Direct3D 11 Texture 2D to use as backbuffer
		
		int									m_outputWidth;
		int									m_outputHeight;
		DXGI_MODE_ROTATION					m_outputRotation;

		D3D_FEATURE_LEVEL					m_d3dFeatureLevel;						// Direct3D feature level
		DXGI_PRESENT_PARAMETERS			    m_dxgiPP;								// DXGI present parameters

		RenderTargetType m_renderTargetType;										// Render Target type (HWND / DXGI / BITMAP)

	private:

		void CreateD3DDevice();

		void CreateD3DResources();

		void OnDeviceLost();

		void CreateD2DDevice();

		void CreateD2DBrushes(ID2D1RenderTarget* pRenderTarget);

		void CreateD2DResources();


	public:
		Graphics(const HWND& hWnd);													// Constructor.. Takes in the handle to the window this graphics class will render to.
			
		~Graphics();																// Destructor. uses the default one.
		
		void Init();

		void Clear();
	
		void Present();

		// Accessor / Mutator functions

		inline void setRenderTargetType(uint16_t type) { m_renderTargetType = RenderTargetType(type); }			// Sets the render target type.

		ID2D1RenderTarget* getRenderTarget();															// Returns the current render target.
		
		ID2D1SolidColorBrush* getSolidColorBrush() { return m_d2d1SolidColorBrush.Get(); }				// Returns the D2d1 solid color brush.
		
		ID2D1LinearGradientBrush* getLinearGradientBrush() { return m_d2d1LinGradientBrush.Get(); }		// Returns the D2d1 linear gradient brush.
				
		ID2D1DeviceContext3* getD2dDeviceContext() { return m_d2d1DeviceContext.Get(); }					// Returns the Direct2D1 Device context.

	};// class
};// namespace

