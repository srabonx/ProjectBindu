#include "Include/Bindu.h"


namespace BINDU
{
	BitmapLoader::BitmapLoader()
	{
		//bitmap = nullptr;
	}
	BitmapLoader::~BitmapLoader()
	{
		//SafeRelease(bitmap);
	}
	bool BitmapLoader::LoadFromFile(const wchar_t* filename, ID2D1Bitmap** ppBitmap)
	{
		ComPtr<IWICImagingFactory> pWicImagingFactory;							// WIC imaging factory
		ComPtr<IWICBitmapDecoder> pDecoder;//= NULL;							// Bitmap Decoder
		ComPtr<IWICBitmapFrameDecode> pFrame;//= NULL;							// Bitmap Frame 
		ComPtr<IWICFormatConverter> pConverter;

		HRESULT hr = S_OK;

		//CoInitializeEx(NULL,COINIT_MULTITHREADED);
		
		// Create WIC factory
		CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)pWicImagingFactory.GetAddressOf());
	
		
		DX::ThrowIfFailed(

			pWicImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder)
	
		);
		
		DX::ThrowIfFailed(

			pDecoder->GetFrame(0, &pFrame)									// Create initial frame	

		);

		DX::ThrowIfFailed(

			pWicImagingFactory->CreateFormatConverter(&pConverter)			// Convert the image format to 32bppPBGRA

		);

		DX::ThrowIfFailed(

			pConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL,
				0.0f, WICBitmapPaletteTypeCustom)

		);

		DX::ThrowIfFailed(

			g_engine->getGraphics()->getRenderTarget()->CreateBitmapFromWicBitmap(pConverter.Get(), NULL, ppBitmap)

		);

		if (ppBitmap)
			(*ppBitmap)->AddRef();

// 		pWicImagingFactory.Reset();
// 		pConverter.Reset();
// 		pDecoder.Reset();
// 		pFrame.Reset();

		return true;
	}
}// Namespace