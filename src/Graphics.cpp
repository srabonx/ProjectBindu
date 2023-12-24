#include "Include/Bindu.h"

namespace BINDU {
	Graphics::Graphics(const HWND& hWnd) :m_windowHandle(hWnd)
	{
		m_d2d1Factory = nullptr;

		m_d2d1Device = nullptr;
		m_d2d1DeviceContext = nullptr;

		m_d2d1HwndRenderTarget = nullptr;
		m_d2d1DxgiRenderTarget = nullptr;
		m_d2d1BitmapTarget = nullptr;

		m_d2d1SolidColorBrush = nullptr;
		m_d2d1LinGradientBrush = nullptr;

		m_dxgiFactory = nullptr;
		m_dxgiDevice = nullptr;
		m_dxgiAdapter = nullptr;
		m_dxgiSwapChain = nullptr;
		m_dxgiSurface = nullptr;
		
		m_d3d11Device = nullptr;
		m_d3d11DeviceContext = nullptr;
		m_d3d11Backbuffer = nullptr;

		ZeroMemory(&m_d3dFeatureLevel, sizeof(m_d3dFeatureLevel));
		m_dxgiPP = { 0 };

		m_renderTargetType = RenderTargetType(0);

	}

	void Graphics::GetDebug(ID3D11Debug** debug)
	{
		m_d3d11Device->QueryInterface(IID_PPV_ARGS(debug));
	}

	Graphics::~Graphics()
	{
		m_d3d11DeviceContext->ClearState();
		m_d3d11DeviceContext->Flush();
		m_d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	

	HRESULT Graphics::Init()
	{
// 		if (FAILED(CreateDeviceIndependentResource()))
// 			g_engine->sendMessage("Error creating device independent resources!", "Error", e_Error::FATAL_ERROR);
// 		
// 		if(FAILED(CreateDeviceResource()))
// 			g_engine->sendMessage("Error creating device resources!", "Error", e_Error::FATAL_ERROR);

		RECT renderTargetRect;

		GetClientRect(m_windowHandle, &renderTargetRect);

		D2D1_SIZE_U size = D2D1::SizeU(renderTargetRect.right - renderTargetRect.left,
			renderTargetRect.bottom - renderTargetRect.top);

		m_outputWidth = size.width;
		m_outputHeight = size.height;
		m_outputRotation = DXGI_MODE_ROTATION_IDENTITY;

		CreateD3DDevice();

		CreateResources();

		return S_OK;
	}

	HRESULT Graphics::BeginDraw()
	{
		HRESULT hr = S_OK;

		// HWND render target begin draw call
		if (m_d2d1HwndRenderTarget && (m_renderTargetType & HWND_RENDERTARGET))
		{
			m_d2d1HwndRenderTarget->BeginDraw();
			m_d2d1HwndRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		// Bitmap render target begin draw call
		if (m_d2d1BitmapTarget && (m_renderTargetType & BITMAP_RENDERTARGET))
		{
			m_d2d1DeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
			m_d2d1DeviceContext->BeginDraw();
			m_d2d1DeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		// DXGI render target begin draw call
		if (m_d2d1DxgiRenderTarget && (m_renderTargetType & DXGI_RENDERTARGET))
		{
			m_d2d1DxgiRenderTarget->BeginDraw();
			m_d2d1DxgiRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		return hr;
	}

// 	void Graphics::Clear(D2D1::ColorF color) 
// 	{
// 		if (m_d2d1HwndRenderTarget)
// 			m_d2d1HwndRenderTarget->Clear(D2D1::ColorF(color));
// 		else if (m_d2d1DxgiRenderTarget)
// 			m_d2d1DxgiRenderTarget->Clear(D2D1::ColorF(color));
// 	}

	HRESULT Graphics::EndDraw()
	{
		HRESULT hr = S_OK;

		if (m_d2d1HwndRenderTarget && (m_renderTargetType & HWND_RENDERTARGET))
		{
			DX::ThrowIfFailed(
				m_d2d1HwndRenderTarget->EndDraw()
			);
		}

		if ((m_renderTargetType & BITMAP_RENDERTARGET) || (m_renderTargetType & DXGI_RENDERTARGET))
		{


			if (m_renderTargetType & DXGI_RENDERTARGET)
				DX::ThrowIfFailed(
					m_d2d1DxgiRenderTarget->EndDraw()
				);

			if (m_renderTargetType & BITMAP_RENDERTARGET)
				DX::ThrowIfFailed(
					m_d2d1DeviceContext->EndDraw()
				);

			DX::ThrowIfFailed(
				m_dxgiSwapChain->Present1(0, 0, &m_dxgiPP)
			);
		}
		
		return hr;
	}

	void Graphics::OnRender()
	{
		BeginDraw();
		//Clear();
		EndDraw();

	}

	HRESULT Graphics::CreateDeviceIndependentResource()
	{
		HRESULT hr = S_OK;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_d2d1Factory.ReleaseAndGetAddressOf());
		
		DX::ThrowIfFailed(
			CreateDXGIFactory(IID_IDXGIFactory,&m_dxgiFactory)
		);
		return hr;
	}

	HRESULT Graphics::CreateDeviceResource()
	{
		HRESULT hr=S_OK;

		// Get the client window size
		RECT renderTargetRect;

		GetClientRect(m_windowHandle, &renderTargetRect);

		D2D1_SIZE_U size = D2D1::SizeU(renderTargetRect.right - renderTargetRect.left,
			renderTargetRect.bottom - renderTargetRect.top);

		// Create HWND Render Target
		if (!m_d2d1HwndRenderTarget && (m_renderTargetType & HWND_RENDERTARGET))
		{

			DX::ThrowIfFailed(
				m_d2d1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),		// Create Direct2D HWND render target
							  D2D1::HwndRenderTargetProperties(m_windowHandle, size),
							  m_d2d1HwndRenderTarget.ReleaseAndGetAddressOf())
			);

			DX::ThrowIfFailed(
				CreateD2dBrushes(m_d2d1HwndRenderTarget.Get())								// Create D2d1 HWND render target brushes
			);
		}

		// DXGI Render Target && BITMAP Render Target
		if ((m_renderTargetType & DXGI_RENDERTARGET) || (m_renderTargetType & BITMAP_RENDERTARGET))
		{
			UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;

			ComPtr<ID3D11Device> pD3d11Device;
			ComPtr<ID3D11DeviceContext> pD3d11DeviceContext;

			hr = CreateD3dDevice(D3D_DRIVER_TYPE_HARDWARE, deviceFlags, &pD3d11Device, &pD3d11DeviceContext);	// Try creating D3d device with hardware support.

			if (FAILED(hr))
			{
				hr = CreateD3dDevice(D3D_DRIVER_TYPE_WARP, deviceFlags, &pD3d11Device, &pD3d11DeviceContext);	// If HW fails. try WARP ( Windows Advanced Rasterization Platform).
			}


// 			DX::ThrowIfFailed(
// 				m_d3d11Device.As(&m_d3d11Debug)								// Obtain Direct3D device underlying debug information
// 			);

			pD3d11Device.As(&m_d3d11Device);
			pD3d11DeviceContext.As(&m_d3d11DeviceContext);

			pD3d11Device->Release();
			pD3d11DeviceContext->Release();

			SetDebugObjectName(m_d3d11DeviceContext.Get(), "D3D11 Device Context");

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };

			swapChainDesc.Width = 0;														// Use automatic sizing
			swapChainDesc.Height = 0;
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;								// This is the common swapchain format
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;												// Don't use multi-sampling
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;													// Use double buffering to enable flip
			swapChainDesc.Scaling = DXGI_SCALING_NONE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			swapChainDesc.Flags = 0;


			ComPtr<IDXGIDevice> pDxgiDevice;

			DX::ThrowIfFailed(
				m_d3d11Device.As(&pDxgiDevice)												// Obtain the underlying DXGI device of the Direct3D11 device
			);

			ComPtr<IDXGIAdapter> pDxgiAdapter;

// 			DX::ThrowIfFailed(
// 				pDxgiDevice->GetAdapter(m_dxgiAdapter.ReleaseAndGetAddressOf())			// Identify the physical adapter (GPU or Card) this device runs on
// 			);

			DX::ThrowIfFailed(
				pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDxgiAdapter)
			);

			ComPtr<IDXGIFactory1> pDxgiFactory;

			DX::ThrowIfFailed(
				pDxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&pDxgiFactory)		// Get the factory object that created the DXGI device
			);


// 			DX::ThrowIfFailed(
// 				m_dxgiFactory->CreateSwapChainForHwnd(m_d3d11Device.Get(),					// Get the final swap chain for this window from DXGI factory
// 					m_windowHandle,
// 					&swapChainDesc, nullptr, nullptr, m_dxgiSwapChain.ReleaseAndGetAddressOf())
// 			);

// 			DX::ThrowIfFailed(
// 				pDxgiFactory->CreateSwapChainForHwnd(m_d3d11Device.Get(),
// 					m_windowHandle, &swapChainDesc, nullptr, nullptr, m_dxgiSwapChain.ReleaseAndGetAddressOf())
// 			);

// 			DX::ThrowIfFailed(
// 				m_dxgiDevice->SetMaximumFrameLatency(1)						// Ensure that DXGI doesn't queue more than one frame at a time
// 			);


// 			DX::ThrowIfFailed(
// 				m_dxgiSwapChain->GetBuffer(0,								// Get backbuffer for this window which is the final 3D render target
// 					IID_PPV_ARGS(m_d3d11Backbuffer.ReleaseAndGetAddressOf()))
// 			);



			ComPtr<ID3D11Texture2D> pTex2DBackbuffer;

			DX::ThrowIfFailed(
				m_dxgiSwapChain->GetBuffer(0,								// Get backbuffer for this window which is the final 3D render target
					IID_PPV_ARGS(pTex2DBackbuffer.ReleaseAndGetAddressOf()))
			);


			// Set up the Direct2D render target bitmap linked to the swapchain.
			// Whenever rendered on this bitmap, it is directly rendered to the
			// swapchain associated with the window

			D2D1_BITMAP_PROPERTIES1 bitmapProperties =
				D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
				);

// 			DX::ThrowIfFailed(
// 				m_dxgiSwapChain->GetBuffer(0,												// Direct2D needs the DXGI version of the backbuffer surface pointer	
// 					IID_PPV_ARGS(m_dxgiSurface.ReleaseAndGetAddressOf()))
// 			);

			ComPtr<IDXGISurface> pDxgiSurface;

			DX::ThrowIfFailed(
				m_dxgiSwapChain->GetBuffer(0,												// Direct2D needs the DXGI version of the backbuffer surface pointer	
					IID_PPV_ARGS(pDxgiSurface.ReleaseAndGetAddressOf()))
			);


			DX::ThrowIfFailed(
				CreateD2dDevice(m_d2d1Device.ReleaseAndGetAddressOf(),pDxgiDevice)								// Obtain Direct2D device for 2-D rendering
			);

			DX::ThrowIfFailed(
				CreateD2dDeviceContext(m_d2d1DeviceContext.ReleaseAndGetAddressOf())				// Get Direct2D device's corresponding device context
			);


			if (m_renderTargetType & BITMAP_RENDERTARGET)
			{
				DX::ThrowIfFailed(
					m_d2d1DeviceContext->CreateBitmapFromDxgiSurface(pDxgiSurface.Get(),	// Get D2D bitmap from the DXGI backbuffer to use as the D2D render target
						&bitmapProperties,
						m_d2d1BitmapTarget.ReleaseAndGetAddressOf())
				);

				m_d2d1DeviceContext->SetTarget(m_d2d1BitmapTarget.Get());					// Set the Direct2D render target
			
				DX::ThrowIfFailed(
					CreateD2dBrushes(m_d2d1DeviceContext.Get())
				);
				
			}


			if (m_renderTargetType & DXGI_RENDERTARGET)
			{
				D2D1_RENDER_TARGET_PROPERTIES renderTargetProps =
					D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
						D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)	// Direct2D render target properties.
					);

				DX::ThrowIfFailed(
					m_d2d1Factory->CreateDxgiSurfaceRenderTarget(pDxgiSurface.Get(),			// Direct2D DXGI render target.
						&renderTargetProps,
						m_d2d1DxgiRenderTarget.ReleaseAndGetAddressOf())
				);

				DX::ThrowIfFailed(
					CreateD2dBrushes(m_d2d1DxgiRenderTarget.Get())
				);

			}

			pDxgiAdapter->Release();
			pDxgiFactory->Release();
			pDxgiDevice->Release();
			pTex2DBackbuffer->Release();
			pDxgiSurface->Release();

			
//			SetDebugObjectName(m_d3d11Backbuffer.Get(), "D3D11 Backbuffer Texture2D");


			m_dxgiPP = { 0,NULL,NULL,NULL };												// DXGI present parameters.
		}
		m_d3d11DeviceContext->Flush();
		return hr;
	}


	HRESULT Graphics::CreateD2dBrushes(ID2D1RenderTarget* renderTarget)
	{
		HRESULT hr = S_OK;

		// Create solid color brush
		if (SUCCEEDED(hr)) {
			hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), m_d2d1SolidColorBrush.ReleaseAndGetAddressOf());
		}

		// Create Linear gradient brush

		ID2D1GradientStopCollection* pGradientStops = NULL;

		D2D1_GRADIENT_STOP gradientStops[2] = { 0 };
		gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1);
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::ForestGreen, 1);
		gradientStops[1].position = 1.0f;
		
		hr = renderTarget->CreateGradientStopCollection(
			gradientStops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGradientStops
		);

		if (SUCCEEDED(hr))
			hr = renderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(150, 150)),
				pGradientStops, m_d2d1LinGradientBrush.ReleaseAndGetAddressOf());

		return hr;
	}

	void Graphics::DiscardDeviceResources()
	{
// 		SafeRelease(m_d2d1SolidColorBrush);
// 		SafeRelease(m_d2d1HwndRenderTarget);
	}

	HRESULT Graphics::CreateD3dDevice(D3D_DRIVER_TYPE driverType,UINT flags,ID3D11Device** ppDevice,ID3D11DeviceContext** ppDeviceContext)
	{
		HRESULT hr = S_OK;
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		DX::ThrowIfFailed(
			D3D11CreateDevice(
				NULL,
				driverType,
				0,
				flags,
				featureLevels,
				ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				ppDevice,
				&m_d3dFeatureLevel,
				ppDeviceContext
			) 
		);

		
		return hr;
	}

	HRESULT Graphics::CreateD2d1HwndRenderTarget(const D2D1_SIZE_U& rtSize, ID2D1HwndRenderTarget** ppHwndRt)
	{
		HRESULT hr = S_FALSE;

		return hr;
	}

	HRESULT Graphics::CreateD2d1DxgiRenderTarget(const D2D1_SIZE_U& rtSize, ID2D1RenderTarget** ppHwndRt)
	{

		return E_NOTIMPL;
	}

	HRESULT Graphics::CreateD2d1BitmapRenderTarget()
	{
		return E_NOTIMPL;
	}

	void Graphics::resizeRenderTarget(int width, int height)
	{
		if (m_d2d1HwndRenderTarget && m_renderTargetType & HWND_RENDERTARGET)
			m_d2d1HwndRenderTarget->Resize(D2D1::SizeU(width, height));
	}

	HRESULT Graphics::CreateD2dDevice(ID2D1Device3** ppDevice,const ComPtr<IDXGIDevice>& dxgiDevice)
	{
		HRESULT hr = S_OK;
		hr = m_d2d1Factory->CreateDevice(dxgiDevice.Get(),ppDevice);
		return hr;
	}

	HRESULT Graphics::CreateD2dDeviceContext(ID2D1DeviceContext3** ppContext)
	{
		HRESULT hr = S_OK;
		hr = m_d2d1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, ppContext);
		
		return hr;
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
	void Graphics::CreateResources()
	{
		// Clear previous window size specific context.
		m_d3d11DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
		m_d3d11RenderTargetView.Reset();
		m_d3d11DepthStencilView.Reset();
		m_d3d11DeviceContext->Flush();


		const UINT backBufferWidth			= static_cast<UINT>(m_outputWidth);
		const UINT backBufferHeight			= static_cast<UINT>(m_outputHeight);
		const DXGI_FORMAT backBufferFormat  = DXGI_FORMAT_B8G8R8A8_UNORM;
		const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		constexpr UINT backBufferCount		= 2;

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

			swapChainDesc.Width					= backBufferWidth;
			swapChainDesc.Height				= backBufferHeight;
			swapChainDesc.Format				= backBufferFormat;
			swapChainDesc.SampleDesc.Count		= 1;
			swapChainDesc.SampleDesc.Quality	= 0;
			swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount			= backBufferCount;
			swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.Scaling				= DXGI_SCALING_STRETCH;
			swapChainDesc.AlphaMode				= DXGI_ALPHA_MODE_IGNORE;

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

	void Graphics::OnDeviceLost()
	{
		// TODO: Add D3D resource cleanup here

		m_d3d11DepthStencilView.Reset();
		m_d3d11RenderTargetView.Reset();
		m_dxgiSwapChain.Reset();
		m_d3d11DeviceContext.Reset();
		m_d3d11Device.Reset();

		CreateD3DDevice();

		CreateResources();
	}

	// Helper method to clear the backbuffer
	void Graphics::Clear()
	{
		// Clear the view
		m_d3d11DeviceContext->ClearRenderTargetView(m_d3d11RenderTargetView.Get(), DirectX::Colors::CornflowerBlue);
		m_d3d11DeviceContext->ClearDepthStencilView(m_d3d11DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_d3d11DeviceContext->OMSetRenderTargets(1, m_d3d11RenderTargetView.GetAddressOf(), m_d3d11DepthStencilView.Get());

		// Set Viewport
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
		m_d3d11DeviceContext->RSSetViewports(1, &viewport);

	}

	void Graphics::Present()
	{

		HRESULT hr = m_dxgiSwapChain->Present(1, 0);

		m_d3d11DeviceContext->DiscardView(m_d3d11RenderTargetView.Get());

		m_d3d11DeviceContext->DiscardView(m_d3d11DepthStencilView.Get());

		// if the device was lost or reset we must completely re-initialize the renderer
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			OnDeviceLost();
		else
			DX::ThrowIfFailed(hr);

	}

};