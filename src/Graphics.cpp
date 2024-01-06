#include "Include/Bindu.h"

namespace BINDU {
	Graphics::Graphics(const HWND& hWnd) :m_windowHandle(hWnd)
	{
		m_d2d1Factory				= nullptr;

		m_d2d1Device				= nullptr;
		m_d2d1DeviceContext			= nullptr;

		m_d2d1HwndRenderTarget		= nullptr;
		m_d2d1DxgiRenderTarget		= nullptr;
		m_d2d1BitmapTarget			= nullptr;

		m_d2d1SolidColorBrush		= nullptr;
		m_d2d1LinGradientBrush		= nullptr;

		m_dxgiSwapChain				= nullptr;
		
		m_d3d11Device				= nullptr;
		m_d3d11DeviceContext		= nullptr;
		m_d3d11Backbuffer			= nullptr;

		m_outputWidth				= 1;
		m_outputHeight				= 1;
		m_outputRotation			= DXGI_MODE_ROTATION_IDENTITY;

		ZeroMemory(&m_d3dFeatureLevel, sizeof(m_d3dFeatureLevel));

		m_dxgiPP					= { 0 };

		m_renderTargetType			 = RenderTargetType(0);

	}

	Graphics::~Graphics()
	{
	#ifdef _DEBUG

		m_d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_SUMMARY);

	#endif // _DEBUG
	}
	

	void Graphics::Init()
	{

		RECT renderTargetRect;

		GetClientRect(m_windowHandle, &renderTargetRect);

		D2D1_SIZE_U size = D2D1::SizeU(renderTargetRect.right - renderTargetRect.left,
			renderTargetRect.bottom - renderTargetRect.top);

		m_outputWidth = size.width;
		m_outputHeight = size.height;
		m_outputRotation = DXGI_MODE_ROTATION_IDENTITY;

		CreateD3DDevice();

		CreateD3DResources();

		CreateD2DDevice();

		CreateD2DResources();
	}


	void Graphics::CreateD3DDevice()
	{
		// This flag add supports for surfaces with different color channel ordering than the API default
		UINT creationFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
		creationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		static const D3D_FEATURE_LEVEL featureLevels[] =
		{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
		};

		m_d3d11Device.Reset();
		m_d3d11DeviceContext.Reset();

		ComPtr<ID3D11Device>				pd3dDevice;
		ComPtr<ID3D11DeviceContext>			pd3dContext;

		DX::ThrowIfFailed(
			D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				creationFlag,
				featureLevels,
				static_cast<UINT>(std::size(featureLevels)),
				D3D11_SDK_VERSION,
				pd3dDevice.GetAddressOf(),
				&m_d3dFeatureLevel,
				pd3dContext.GetAddressOf()
			)
		);

		DX::ThrowIfFailed(pd3dDevice.As(&m_d3d11Device));
		DX::ThrowIfFailed(pd3dContext.As(&m_d3d11DeviceContext));

		SetDebugObjectName(m_d3d11DeviceContext.Get(), "_DEBUG D3D11 Device Context");

#ifndef NDEBUG

		ComPtr<ID3D11Debug> pd3dDebug;
		if (SUCCEEDED(m_d3d11Device.As(&pd3dDebug)))
		{
			//pd3dDebug.As(&m_d3d11Debug);
			ComPtr<ID3D11InfoQueue> pd3d11InfoQueue;
			if (SUCCEEDED(pd3dDebug.As(&pd3d11InfoQueue)))
			{
#ifdef _DEBUG
				pd3d11InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				pd3d11InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
					// TODO: Add more message ID's as needed
				};

				D3D11_INFO_QUEUE_FILTER filter = {};
				filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
				filter.DenyList.pIDList = hide;
				pd3d11InfoQueue->AddStorageFilterEntries(&filter);
			}
		}
#endif

		// TODO: Initialize device dependent resources here (independent of window size).
		pd3dDebug.As(&m_d3d11Debug);
	}


	// Allocate all memory resources that change on window SizeChanged event.
	void Graphics::CreateD3DResources()
	{
		// Clear previous window size specific context.
		m_d3d11DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_d3d11RenderTargetView.Reset();
		m_d3d11DepthStencilView.Reset();
		m_d3d11DeviceContext->Flush();


		const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
		const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
		const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
		const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		constexpr UINT backBufferCount = 2;

		// If swapchain already exist, resize it, otherwise create one
		if (m_dxgiSwapChain)
		{
			HRESULT hr = S_OK;

			hr = m_dxgiSwapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			{
				// If the device was removed for any reason, a new swap chain will be created.
				OnDeviceLost();

				// Everything is set up now. Do not continue execution of this method. OnDeviceLost will re-enter this
				// method and correctly set up the new device.

				return;
			}
			else
			{
				DX::ThrowIfFailed(hr);
			}
		}
		else
		{

			// Retrieve the underlying DXGI device from the D3D device.
			ComPtr<IDXGIDevice1> pdxgiDevice;
			DX::ThrowIfFailed(m_d3d11Device.As(&pdxgiDevice));

			// Identify the physical adapter (GPU or card) this device is running on.
			ComPtr<IDXGIAdapter> pdxgiAdapter;
			DX::ThrowIfFailed(pdxgiDevice->GetAdapter(pdxgiAdapter.GetAddressOf()));

			// And obtain the factory object that created it.
			ComPtr<IDXGIFactory2> pdxgiFactory;
			DX::ThrowIfFailed(pdxgiAdapter->GetParent(IID_PPV_ARGS(pdxgiFactory.GetAddressOf())));

			// Create descriptor for the swap chain
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backBufferCount;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

			ComPtr<IDXGISwapChain1> pdxgiSwapChain;
			DX::ThrowIfFailed(pdxgiFactory->CreateSwapChainForHwnd(
				m_d3d11Device.Get(),
				m_windowHandle,
				&swapChainDesc,
				nullptr,
				nullptr,
				pdxgiSwapChain.GetAddressOf()
			));

			DX::ThrowIfFailed(pdxgiSwapChain.As(&m_dxgiSwapChain));

			DX::ThrowIfFailed(pdxgiDevice->SetMaximumFrameLatency(1));

		}

		DX::ThrowIfFailed(m_dxgiSwapChain->SetRotation(m_outputRotation));

		// Obtain the backbuffer for this window which will be the final 3D render target.
		ComPtr<ID3D11Texture2D> backBuffer;
		DX::ThrowIfFailed(m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

		// Create a view interface on the render target to use on bind
		DX::ThrowIfFailed(m_d3d11Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_d3d11RenderTargetView.ReleaseAndGetAddressOf()));

		// Allocate a 2D surface as the Depth/Stencil buffer.
		// Create a depth/stencil view on this surface to use on bind
		CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

		ComPtr<ID3D11Texture2D> depthStencil;
		DX::ThrowIfFailed(m_d3d11Device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		DX::ThrowIfFailed(m_d3d11Device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_d3d11DepthStencilView.ReleaseAndGetAddressOf()));

		// TODO: Initialize window size dependent objects here.

	}


	void Graphics::CreateD2DDevice()
	{
		D2D1_THREADING_MODE d2d1ThreadMode				 = D2D1_THREADING_MODE_SINGLE_THREADED;
		D2D1_DEBUG_LEVEL d2d1DebugLvl					 = D2D1_DEBUG_LEVEL_NONE;
		D2D1_DEVICE_CONTEXT_OPTIONS d2d1ContextOption	 = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

	#ifdef _DEBUG
		
		d2d1DebugLvl									 = D2D1_DEBUG_LEVEL_INFORMATION;
		
	#endif // _DEBUG


		D2D1_CREATION_PROPERTIES props =
		{
			d2d1ThreadMode,
			d2d1DebugLvl,
			d2d1ContextOption
		};

		D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);

		ComPtr<IDXGISurface> pdxgiSurface;
		DX::ThrowIfFailed(m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&pdxgiSurface)));

		ComPtr<ID2D1DeviceContext> pd2d1DeviceContext;

		DX::ThrowIfFailed(D2D1CreateDeviceContext(pdxgiSurface.Get(), props, pd2d1DeviceContext.GetAddressOf()));

		DX::ThrowIfFailed(pd2d1DeviceContext.As(&m_d2d1DeviceContext));

		ComPtr<ID2D1Bitmap1> pd2d1Bitmap;
		DX::ThrowIfFailed(
			m_d2d1DeviceContext->CreateBitmapFromDxgiSurface(pdxgiSurface.Get(), bitmapProps, pd2d1Bitmap.GetAddressOf())
		);

		m_d2d1DeviceContext->SetTarget(pd2d1Bitmap.Get());

		m_d2d1DeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	}


	void Graphics::CreateD2DResources()
	{
		CreateD2DBrushes(m_d2d1DeviceContext.Get());					// Create the brushes.
	}


	void Graphics::CreateD2DBrushes(ID2D1RenderTarget* pRenderTarget)
	{
		// Create solid color brush
		ComPtr<ID2D1SolidColorBrush> pSolidColorBrush;
		DX::ThrowIfFailed(
			pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), pSolidColorBrush.GetAddressOf())
		);

		DX::ThrowIfFailed(pSolidColorBrush.As(&m_d2d1SolidColorBrush));

		// Create Linear gradient brush

		ComPtr<ID2D1GradientStopCollection> pGradientStops;
		ComPtr<ID2D1LinearGradientBrush> pLinearGradientBrush;

		D2D1_GRADIENT_STOP gradientStops[2] = { 0 };
		gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::ForestGreen, 1);
		gradientStops[1].position = 1.0f;
		
		DX::ThrowIfFailed(pRenderTarget->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGradientStops
		));

		DX::ThrowIfFailed(pRenderTarget->CreateLinearGradientBrush(
			D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(150, 150)),
			pGradientStops.Get(), pLinearGradientBrush.GetAddressOf()));

		//DX::ThrowIfFailed(pLinearGradientBrush.As(&m_d2d1LinGradientBrush));

	}


	void Graphics::OnDeviceLost()
	{
		// TODO: Add D3D resource cleanup here

		m_d2d1DeviceContext.Reset();
		m_d3d11DepthStencilView.Reset();
		m_d3d11RenderTargetView.Reset();
		m_dxgiSwapChain.Reset();
		m_d3d11DeviceContext.Reset();
		m_d3d11Device.Reset();

		CreateD3DDevice();

		CreateD3DResources();

		CreateD2DDevice();
	}


	// Helper method to clear the backbuffer
	void Graphics::Clear()
	{
		// Clear the view
//		m_d3d11DeviceContext->ClearRenderTargetView(m_d3d11RenderTargetView.Get(), DirectX::Colors::CornflowerBlue);
//		m_d3d11DeviceContext->ClearDepthStencilView(m_d3d11DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

//		m_d3d11DeviceContext->OMSetRenderTargets(1, m_d3d11RenderTargetView.GetAddressOf(), m_d3d11DepthStencilView.Get());

		// Set Viewport
//		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
//		m_d3d11DeviceContext->RSSetViewports(1, &viewport);
		
		m_d2d1DeviceContext->BeginDraw();
		m_d2d1DeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));

	}

	void Graphics::Present()
	{
		HRESULT hr = S_OK;
		hr = m_d2d1DeviceContext->EndDraw();
		hr = m_dxgiSwapChain->Present(1, 0);

		//		m_d3d11DeviceContext->DiscardView(m_d3d11RenderTargetView.Get());

		//		m_d3d11DeviceContext->DiscardView(m_d3d11DepthStencilView.Get());

				// if the device was lost or reset we must completely re-initialize the renderer
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			OnDeviceLost();
		else
			DX::ThrowIfFailed(hr);

	}


	ID2D1RenderTarget* Graphics::getRenderTarget()
	{
		if (m_renderTargetType & HWND_RENDERTARGET)
			return m_d2d1HwndRenderTarget.Get();
		if (m_renderTargetType & DXGI_RENDERTARGET)
			return m_d2d1DxgiRenderTarget.Get();
		if (m_renderTargetType & BITMAP_RENDERTARGET)
			return m_d2d1DeviceContext.Get();
		return nullptr;
	}



};